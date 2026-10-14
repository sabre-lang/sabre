/// Node Modules
import * as fs from 'node:fs';
import * as path from 'node:path';

/// Vendor Modules
import * as JSONC from 'jsonc-parser';

/// Website Modules
import { TOC } from '@/website/components';
import { Assets } from '@/website/assets';
import { Product } from '@/website/product';
import { Docify } from '@/website/docify';

/// Package Modules
import { Manifest } from '../manifest';

/** Crates Registry Namespace. */
export namespace Registry {
    //  PUBLIC METHODS  //

    /** Resolves a list of all crates. */
    export function list(): Manifest[] {
        // prepare the source directory of crates
        const source = Assets.crates();

        // filter all the available files now
        return fs
            .readdirSync(source)
            .map(resolve)
            .filter((crate): crate is Manifest => typeof crate === 'object');
    }

    /**
     * Handles resolving crate manifests.
     * @param name                  Name of crate.
     */
    export function resolve(name: string): Manifest | undefined {
        const prefix = `${Product.identifier}:`; // prepare the prefix now
        name = name.startsWith(prefix) ? name.slice(prefix.length) : name;

        // attempt finding the necessary crate now
        const crate = Assets.crates(name, '_crate.jsonc');
        if (name.startsWith('_') || !fs.existsSync(crate)) return;

        // prepare a suitable resolution handler now
        const resolve = (...segments: string[]) => path.resolve(crate, '..', ...segments);

        // attempt parsing the incoming JSONC file now
        return { ...m_details(name, crate), resource: crate, resolve };
    }

    /**
     * Handles transform source table-of-contents.
     * @param source                Source to transform.
     */
    export function transform(source: typeof import('@/website/source').Source.crates, prefix = 'crates') {
        // prepare a set of keyed items
        const keys = new Set<string>();
        const index = `${prefix}/index.mdx`;

        // iteratively update the source files now
        for (const file of source.files) {
            // ignore invalid file types
            if (file.type === 'meta' || file.path === index) continue;

            // prepare the key to be used
            const key = file.path.split('/')[1];
            const { project } = resolve(key) ?? {};

            // bind the key and table as necessary now
            (keys.add(key), (file.data.toc = m_seed(project)));
        }

        // attempt removing the collapsible items
        for (const key of keys) source.files.push(TOC.meta(key, prefix));
    }

    //  PRIVATE METHODS  //

    /**
     * Handles parsing crate details.
     * @param name                  Name of crate.
     * @param crate                 Crate file-path.
     */
    function m_details(name: string, crate: string): Manifest {
        // parse the current crate details and project details
        const details = JSONC.parse(fs.readFileSync(crate, 'utf-8'));

        // prepare the docify project as well to be used
        const project = Assets.crates(name, '_docify.jsonc');
        if (!fs.existsSync(project)) return details;

        // can safely resolve both details now
        return { ...details, project: JSONC.parse(fs.readFileSync(project, 'utf-8')) };
    }

    /**
     * Handles seeding the TOC values.
     * @param project               Project documentation.
     */
    function m_seed(project: Docify.Project | undefined): TOC {
        // prepare the overview header to be used
        const header = [TOC.seed('Overview')];

        // ignore if current invalid at all
        if (typeof project === 'undefined') return header;

        // prepare the seed handler for children
        const seed = (section: string, identifier: string) => {
            const name = project.nodes[identifier].name;
            return TOC.seed(name, { url: `#${section}-${name.toLowerCase()}`, depth: 3 });
        };

        // iterate over the available groups to be output
        return header.concat(
            project.groups.flatMap((group) => {
                if (group.children.length === 0) return [];
                const level = seed.bind(undefined, group.title.toLowerCase());
                const children = group.children.map(level);
                return [TOC.seed(group.title)].concat(children);
            }),
        );
    }
}
