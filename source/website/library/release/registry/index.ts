/// Website Modules
import { Product } from '@/website/product';
import { Source } from '@/website/source';

/** Release Registry Details. */
export type Details = ReturnType<typeof Source.blog.getPages>[number];

/** Release Registry Functionality. */
export namespace Registry {
    //  PROPERTIES  //

    /** Expected release prefix. */
    export const prefix = `${Product.shortName} v`;

    //  PUBLIC METHODS  //

    /** Gets a list of available release pages. */
    export function list() {
        return Source.blog
            .getPages()
            .filter(validate)
            .sort((a, b) => a.path.localeCompare(b.path));
    }

    /**
     * Handles validating pages.
     * @param page                  Page to validate.
     */
    export function validate(page?: Details): page is Details {
        return !!page?.data.title.startsWith(prefix);
    }

    /**
     * Handles validating renames.
     * @param page                  Page to validate.
     */
    export function rename(page: Details) {
        return page.data.title.slice(prefix.length) < '0.3.1';
    }
}
