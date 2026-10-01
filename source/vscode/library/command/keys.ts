/// VSC Modules
import { Product } from '@/vscode/product';

/** Constructs a pre-defined command key. */
export function Key<K extends string>(key: K): `${string}.${K}` {
    return `${Product.identifier}.${key}`;
}

export namespace Key {
    //  PROPERTIES  //

    /** Server Restart Key. */
    export const RESTART_SERVER = Key('restart');

    /** Open Settings Key. */
    export const SETTINGS_OPEN = Key('settings');
}
