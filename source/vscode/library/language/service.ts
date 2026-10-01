/// VSC Modules
import { Command } from '@/vscode/command';
import { Settings } from '@/vscode/settings';
import { Extension } from '@/vscode/extension';
import { Decorator, Disposable, Inversify } from '@/vscode/utilities';

/// Package Modules
import { Client } from './client';

/** Language Service Contribution. */
@Inversify.injectable()
@Decorator.Class.Rename('Language.Service')
export class Service extends Disposable.Registry implements Extension.Plugin {
    //  INJECTABLES  //

    /** Language Client Instance. */
    @Inversify.inject(Client) protected readonly m_client!: Client;

    /** Commands Service. */
    @Inversify.inject(Command.Service) protected readonly m_commands!: Command.Service;

    //  LIFECYCLE METHODS  //

    /** Handles configuring the instance. */
    async configure() {
        // register the base client instance
        this.m_subscribe(this.m_client);

        // prepare all the necessary event listeners
        this.m_commands.register(Command.Key.RESTART_SERVER, () => this.m_client.restart());

        // handle when the underlying settings change (eg: restart the server instance)
        this.m_subscribe(Settings.Registry.listen('server.loggingLevel', () => this.m_client.restart()));
        this.m_subscribe(Settings.Registry.listen('server.executablePath', () => this.m_client.restart()));

        // finally force the client to be initialized first
        await this.m_client.restart();
    }

    //  PUBLIC METHODS  //

    /** Handles restarting the language service. */
    async restart(): Promise<void> {
        return this.m_client.restart();
    }

    /** Makes a client request. */
    async request(...args: any[]) {
        return (<any>this.m_client.request)(...args);
    }

    /** Sends a client notification. */
    async notify(...args: any[]) {
        return (<any>this.m_client.notify)(...args);
    }
}
