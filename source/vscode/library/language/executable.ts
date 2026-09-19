/// Node Modules
import * as os from 'node:os';
import * as fs from 'node:fs';
import * as path from 'node:path';

/// Vendor Modules
import * as vscode from 'vscode';

/// VSC Modules
import { Product } from '@/vscode/product';
import { Settings } from '@/vscode/settings';

/** Gets the LSP server executable. */
export namespace Executable {
    //  PROPERTIES  //

    /** Expected base locations available. */
    const m_locations = [
        // as well as a development path as well (eg: located against build outputs)
        path.resolve(__dirname, '..', 'bin', Product.executable),

        // prepare a valid location for the potential executable
        path.resolve(os.homedir(), '.sabre', 'bin', Product.executable),
        path.resolve(os.homedir(), '.talos', 'bin', `talos${process.platform === 'win32' ? '.exe' : ''}`),
    ];

    //  PUBLIC METHODS  //

    /** Gets the executable name. */
    export const name = () => Product.executable;

    /** Resolves a desired executable. */
    export const resolve = () => {
        const user = Settings.Registry.get('server.executablePath').trim();
        const locations = user ? [user].concat(m_locations) : m_locations;
        return locations.filter((location) => fs.existsSync(location)).at(0);
    };

    /** Handles missing installations. */
    export const missing = () => {
        const message = `Could not find "${Product.executable}" executable`;
        vscode.window.showErrorMessage(message, 'Install', 'Ignore').then(async (result) => {
            if (result === 'Install') return m_install().catch(m_failure);
        });
    };

    //  PRIVATE METHODS  //

    /** Handles manually downloading the executable. */
    async function m_install() {
        // prepare the baseline details about the installation
        const location = vscode.ProgressLocation.Notification;
        const title = `Installing "${Product.shortName}" from "https://sabre.rroessler.io/"`;

        // and execute the installation with a progress display
        await vscode.window.withProgress({ title, location }, async (_, token) => {
            // prepare the installation command to be used
            const command =
                process.platform === 'win32'
                    ? 'powershell -c "irm https://sabre.rroessler.io/install.ps1 | iex"'
                    : 'curl -fsSL https://sabre.rroessler.io/install.sh | bash';

            // prepare the command we want to execute
            const execution = new vscode.ShellExecution(command);

            // prepare the task to handle the execution
            const task = new vscode.Task(
                { type: 'shell' },
                vscode.TaskScope.Workspace,
                `Install "${Product.shortName}"`,
                Product.identifier,
                execution,
            );

            // attempt running the execution now
            const result = await vscode.tasks.executeTask(task);

            // terminate the result if necessary
            token.onCancellationRequested(() => result.terminate());

            // wrap everything in a suitable listener to await completion
            return new Promise<void>((resolve, reject) => {
                const disposable = vscode.tasks.onDidEndTaskProcess((event) => {
                    // ignore invalid execution results
                    if (event.execution !== result) return;

                    // since valid, we want to remove this listener
                    disposable.dispose();

                    // and handle the final result now
                    event.exitCode ? reject(new Error('Command ended with non-zero exit-code')) : resolve();
                });
            });
        });
    }

    /**
     * Formats a failure message.
     * @param error                 Error to format.
     */
    function m_failure(error: any) {
        return vscode.window.showErrorMessage(`Failed to install "${Product.shortName}": ${error}`);
    }
}
