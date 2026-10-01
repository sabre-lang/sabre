/// Vendor Modules
import * as vscode from 'vscode';

/// JSON Modules
import _ from '~/configs/extension.json';

/** Product Constants. */
export namespace Product {
    //  PROPERTIES  //

    /** Underlying product identifier. */
    export const identifier = _.name;

    /** Underlying product publisher. */
    export const publisher = _.publisher;

    /** Underlying crate file-name. */
    export const crateFile = '_crate.jsonc';

    /** Short descriptor name. */
    export const shortName = identifier[0].toUpperCase() + identifier.slice(1);

    /** Long descriptor name. */
    export const longName = _.displayName;

    /** Executable name expected. */
    export const executable = `${identifier}${process.platform === 'win32' ? '.exe' : ''}`;

    /** Document Filter Selector. */
    export const selector = { scheme: 'file', language: identifier } satisfies vscode.DocumentSelector;
}
