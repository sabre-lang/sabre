/// Vendor Modules
import * as fumadocs from 'fumadocs-mdx/config';

/// Website Modules
import { Assets } from '@/website/assets';
import { Language } from '@/website/language';

/// Package Modules
import { Schema } from '@/website/schema';

/** Blogs Collection. */
export const blog = fumadocs.defineCollections({
    type: 'doc',
    dir: Assets.blog(),
    schema: Schema.page.extend({ author: Schema.author, date: Schema.date }),
});

/** Proposals Collection. */
export const proposals = fumadocs.defineCollections({
    type: 'doc',
    dir: Assets.docs('proposals'),
    schema: Schema.page.extend({ author: Schema.author, revision: Schema.revision }),
});

export const builtins = m_define(Assets.builtins());
export const crates = m_define(Assets.crates(), '!README.md');
export const docs = m_define(Assets.docs(), '!blog', '!internal', '!proposals');

/** Fumadocs Configuration */
export default fumadocs.defineConfig({
    mdxOptions: {
        providerImportSource: '@/website/markdown/_bootstrap',
        rehypeCodeOptions: {
            themes: Language.Themes,
            langs: Language.Grammar(),
            transformers: [Language.Transformer.Deprecated()],
        },
    },
});

//  PRIVATE METHODS  //

/**
 * Handles defining documetation collections.
 * @param dir               Directory to bind.
 */
function m_define(dir: string, ...files: string[]) {
    return fumadocs.defineDocs({
        dir,
        meta: { files: ['**/meta.json'], schema: Schema.meta },
        docs: {
            schema: Schema.page,
            files: ['**/*.md', '**/*.mdx', ...files],
            postprocess: { includeProcessedMarkdown: true },
        },
    });
}
