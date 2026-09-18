/// Vendor Modules
import { Heading } from 'fumadocs-ui/components/heading';

/// Website Modules
import { Markdown } from '@/website/markdown';
import { Anchor, Code } from '@/website/components';

/** Builtins Property Component. */
export interface Property extends Property.Props {}
export function Property({ name, parent, describe, typedef, ancillary }: Property) {
    // prepare the href to be used now
    const href = `#${parent ? `${parent}-` : ''}${name}`;

    // prepare the top-most link to be used
    const title = <Anchor href={href} children={<code>.{name}</code>} />;

    // construct the resulting fragments to be used now
    return (
        <div className="mb-4">
            <Heading as="h3" id={href.slice(1)} children={title} />
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
