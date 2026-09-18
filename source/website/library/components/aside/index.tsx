/// Package Modules
import { Callout } from '../callout';

/** Common Aside Components. */
export namespace Aside {
    //  PROPERTIES  //

    /** Rewrite Aside Component. */
    export function Rewrite() {
        return (
            <Callout>
                The rewrite for Sabre is currently being implemented privately to keep the current project state clean.
                I have done this since some sections are being rewriten potentially from the ground-up, which will
                simplify with the adoption of these substantial changes.
            </Callout>
        );
    }
}
