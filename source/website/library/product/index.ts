/// JSON Modules
import { description as d } from '../../package.json';
import { publisher, displayName } from '~/configs/extension.json';

/** The Product Details. */
export namespace Product {
    //  PROPERTIES  //

    /** Underlying product identifier. */
    export const identifier = publisher;

    /** Available extension typing. */
    export const extension = '.sabre';

    /** The baseline github URL. */
    export const github = 'https://github.com/rroessler/sabre-lang';

    /** Assign the associated website description. */
    export const description = d;

    /** Short descriptor name. */
    export const shortName = identifier[0].toUpperCase() + identifier.slice(1);

    /** Long descriptor name. */
    export const longName = displayName;

    /** Denotes the latest Sabre version. */
    export const version = process.env.SABRE_VERSION?.substring(1);

    /** Checks if currently in development. */
    export const development = process.env.NODE_ENV === 'development';
}
