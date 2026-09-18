/// Vendor Modules
import * as vscode from 'vscode';
import * as vslc from 'vscode-languageclient/node';
import * as lsp from 'vscode-languageserver-protocol';

/// VSC Modules
import { Output } from '@/vscode/output';
import { Product } from '@/vscode/product';
import { Settings } from '@/vscode/settings';
import { Decorator, Disposable, Inversify } from '@/vscode/utilities';

/// Package Modules
import { Executable } from './executable';

/** Connection Details. */
export type Connection = vslc.LanguageClient;
export namespace Callback {
    //  TYPEDEFS  //

    export type Generic = (...args: any[]) => any;
    export type Pending = [Generic, Disposable];
    export type Register = (method: string, callback: (...args: any[]) => any) => Disposable;
}

@Inversify.injectable()
@Decorator.Class.Rename('Language.Client')
export class Client extends Disposable {
    //  PROPERTIES  //

    /** Language Client Connection. */
    private m_connection?: vslc.LanguageClient;

    /** Pending requests to be bound. */
    private m_requests = new Map<string | lsp.MessageSignature, Callback.Pending>();

    /** Pending notifications to be bound. */
    private m_notifications = new Map<string | lsp.MessageSignature, Callback.Pending>();

    //  LIFECYLE METHODS  //

    /**
     * Constructs a language client.
     * @param m_output                      Output service.
     */
    constructor(@Inversify.inject(Output.Service) private readonly m_output: Output.Service) {
        (super(), this.restart());
    }

    /** Handles disposing of the connection. */
    dispose() {
        this.m_connection?.dispose();
        this.m_connection = undefined;
    }

    //  PUBLIC METHODS  //

    /** Handles restarting the language client. */
    async restart() {
        // attempt getting the current options
        const options = this.m_options();
        const connection = this.m_resolve(options);

        // and handle as necessary now
        await this.m_restart(connection);
    }

    /**
     * Handles emitting a request.
     * @param type                          Type of request.
     * @param ...params                     Params to emit.
     */
    request<R, PR, E, RO>(type: lsp.ProtocolRequestType0<R, PR, E, RO>, token?: any): Promise<R>;
    request<P, R, PR, E, RO>(type: lsp.ProtocolRequestType<P, R, PR, E, RO>, params: P, token?: any): Promise<R>;
    request<R, E>(type: lsp.RequestType0<R, E>, token?: any): Promise<R>;
    request<P, R, E>(type: lsp.RequestType<P, R, E>, params: P, token?: any): Promise<R>;
    request<R>(method: string, token?: any): Promise<R>;
    request<R>(method: string, param: any, token?: any): Promise<R>;
    async request<R>(type: string | lsp.MessageSignature, ...params: any[]): Promise<R> {
        if (this.m_connection) return this.m_connection.sendRequest<R>(type as any, ...params);
        throw new lsp.ResponseError(lsp.ErrorCodes.ConnectionInactive, 'Client is not running');
    }

    /**
     * Handles sending a notification.
     * @param type                          Type of notification.
     * @param params                        Parameters to emit.
     */
    notify<RO>(type: lsp.ProtocolNotificationType0<RO>): Promise<void>;
    notify<P, RO>(type: lsp.ProtocolNotificationType<P, RO>, params: P): Promise<void>;
    notify(type: lsp.NotificationType0): Promise<void>;
    notify<P>(type: lsp.NotificationType<P>, params: P): Promise<void>;
    notify(method: string): Promise<void>;
    notify(method: string, params: any): Promise<void>;
    async notify<P>(type: string | lsp.MessageSignature, params?: P): Promise<void> {
        if (this.m_connection) return this.m_connection.sendNotification(type as any, params);
        throw new lsp.ResponseError(lsp.ErrorCodes.ConnectionInactive, 'Client is not running');
    }

    //  PRIVATE METHODS  //

    /**
     * Constructs desired server options.
     * @param command                       Command to bind.
     */
    private m_options(): vslc.ServerOptions | undefined {
        // ensure the executable actually exists first
        const command = Executable.resolve();
        if (typeof command === 'undefined') return;

        // get the necessary environment variables that we enable now
        const SABRE_LOGGING_LEVEL = Settings.Registry.get('server.loggingLevel');

        // prepare the environment now
        const env = { ...process.env, SABRE_LOGGING_LEVEL, NO_COLOR: '1' };

        // prepare the options to be inherited
        const options: vslc.ExecutableOptions = { env };

        // and construct the resulting server options
        return { command, args: ['serve'], transport: vslc.TransportKind.stdio, options };
    }

    /**
     * Handles resolving a connection instance.
     * @param options                       Options to set.
     */
    private m_resolve(options: vslc.ServerOptions | undefined) {
        // ignore the underlying options instance now
        if (typeof options === 'undefined') return;

        // attempt resolving a suitable connection now
        this.m_connection ??= new vslc.LanguageClient(Product.identifier, options, {
            documentSelector: [Product.selector],
            outputChannel: this.m_output.channel(),
            diagnosticCollectionName: Product.identifier,
            synchronize: { fileEvents: this.m_watchers() },
            markdown: { isTrusted: true, supportHtml: true },
            revealOutputChannelOn: vslc.RevealOutputChannelOn.Never,
        });

        // append all the pending notification and request handlers
        this.m_pending(this.m_requests, this.m_connection.onRequest.bind(this.m_connection));
        this.m_pending(this.m_notifications, this.m_connection.onNotification.bind(this.m_connection));

        // update the connection options to be used now before returing
        return (((<any>this.m_connection['_serverOptions']) = options), this.m_connection);
    }

    /** Gets all the available file-watchers to be used. */
    private m_watchers(): vscode.FileSystemWatcher[] {
        return [Product.crateFile].map((fileName) => vscode.workspace.createFileSystemWatcher(`**/${fileName}`));
    }

    /**
     * Handles updating pending items.
     * @param items                     Pending items.
     * @param register                  Registry callback.
     */
    private m_pending(items: Map<any, Callback.Pending>, register: Callback.Register) {
        for (const [method, [item, disposable]] of items) disposable.dispose = register(method, item).dispose;
        items.clear(); // clear the items that were pending now as necessary
    }

    private async m_restart(connection: vslc.LanguageClient | undefined) {
        if (typeof connection === 'undefined') Executable.missing();
        else return connection.isRunning() ? connection.restart() : connection.start();
    }
}
