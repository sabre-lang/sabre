/// Sabre Modules
import { Dependency, Disposable } from '../utilities';

/** Extension Plugin Interface. */
export const Plugin = Dependency.Name('extension.plugin');
export interface Plugin extends Disposable.Maybe {
    readonly constructor: NewableFunction;

    configure?(): Promise<void>;
    activate?(): Promise<void>;
}

export namespace Plugin {
    //  TYPEDEFS  //

    /** Available Plugin Actions. */
    export type Action = keyof Omit<Plugin, 'constructor' | 'dispose'>;
}
