/// Vendor Modules
import { Heading } from 'fumadocs-ui/components/heading';

/// Website Modules
import { Code } from '@/website/components';
import { Markdown } from '@/website/markdown';

/** Builtins Property Component. */
export interface Property extends Property.Props {}
export function Property({ name, parent, describe, typedef, ancillary }: Property) {
    // prepare the href to be used now
    const href = `#${parent ? `${parent}-` : ''}${name}`;

    // construct the resulting fragments to be used now
    return (
        <div className="mb-4">
            <Heading as="h3" id={href.slice(1)} children={<code>.{name}</code>} />
            {typedef && <Code.Dynamic lang="sabre" code={typedef} />}
            <Markdown.Dynamic markdown={describe} />
            {ancillary /** additional details */}
        </div>
    );
}

export namespace Property {
    //  TYPEDEFS  //

    /** Property Component Properties. */
    export interface Props {
        readonly name: string;
        readonly parent?: string;
        readonly typedef?: string;
        readonly describe?: string;
        readonly ancillary?: React.ReactNode;
    }
}
