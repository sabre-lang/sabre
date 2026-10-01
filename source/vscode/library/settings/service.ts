/// Vendor Modules
import * as vscode from 'vscode';

/// VSC Modules
import { Product } from '@/vscode/product';
import { Command } from '@/vscode/command';
import { Extension } from '@/vscode/extension';
import { Decorator, Disposable, Inversify } from '@/vscode/utilities';

/** Settings Service. */
@Inversify.injectable()
@Decorator.Class.Rename('Settings.Service')
export class Service extends Disposable.Registry implements Extension.Plugin {
    //  INJECTABLES  //

    /** Commands Service. */
    @Inversify.inject(Command.Service) protected readonly m_commands!: Command.Service;

    //  LIFECYCLE METHODS  //

    /** Handles opening user-settings. */
    async open() {
        await vscode.commands.executeCommand(
            'workbench.action.openSettings',
            `@ext:${Product.publisher}.${Product.identifier}`,
        );
    }

    /** Handles configuring the service. */
    async configure() {
        this.m_commands.register(Command.Key.SETTINGS_OPEN, this.open.bind(this));
    }
}
