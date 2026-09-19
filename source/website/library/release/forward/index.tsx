/// Package Modules
import { Details, Registry } from '../registry';

/// Markdown Modules
import Rename from './_rename.mdx';
import Snippet from './_snippet.mdx';

/** Preview Component. */
export interface Forward extends Forward.Props {}
export function Forward({ page }: Forward) {
    if (!Registry.validate(page)) return;
    const rename = Registry.rename(page);
    return rename ? <Rename /> : <Snippet />;
}

export namespace Forward {
    //  TYPEDEFS  //

    /** Preview Component Properties. */
    export interface Props {
        readonly page: Details;
    }
}
