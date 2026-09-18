/// Node Modules
import * as fs from 'node:fs';

/// Vendor Modules
import { SwordIcon } from 'lucide-react';

/// Library Modules
import { Assets } from '@/website/assets';

/// Package Modules
import { Code } from '../code';

/** Code Example Component. */
export interface Example extends Example.Props {}
export async function Example({ href, lang, ...props }: Example) {
    // ignore if the incoming reference is invalid
    if (typeof href === 'undefined') return null;

    // attempt resolving the incoming path to be used
    const example = await fs.promises.readFile(Assets.examples(href), 'utf-8');

    // resolve a suitable icon as necessary now
    const icon = lang === 'sabre' ? <SwordIcon size="16" /> : undefined;

    // resolve a title as necessary now
    const title: any = href.includes('/') ? href : `examples/${href}`;

    // render the incoming code now as necessary
    return <Code.Dynamic icon={icon} code={example.trim()} title={title} lang={lang ?? 'text'} {...props} />;
}

export namespace Example {
    //  TYPEDEFS  //

    /** Example Component Properties. */
    export type Props = Code.Block & { href?: string; lang?: string };
}
