/// Vendor Modules
import type * as shiki from 'shiki';
import * as react from 'react/jsx-runtime';
import * as hast from 'hast-util-to-jsx-runtime';
import { createShikiFactory } from 'fumadocs-core/highlight/shiki';

/// Package Modules
import { Grammar } from './grammar';
import { Transformer } from './transformer';

/** Available Theming. */
export type Themes = shiki.CodeOptionsMultipleThemes['themes'];
export const Themes = { light: 'github-light', dark: 'github-dark' } as const satisfies Themes;

/** Code Highlighting Funtionality. */
export namespace Highlight {
    //  TYPEDEFS  //

    export type Engine = 'js' | 'wasm';
    export type Options = shiki.CodeOptionsMeta &
        Omit<shiki.CodeToHastOptionsCommon, 'lang' | 'theme'> & {
            lang?: string;
            engine?: Engine;
            components?: Partial<hast.Components>;
        };

    //  PROPERTIES  //

    const m_js = m_factory((details) => async () => details.createJavaScriptRegexEngine());
    const m_wasm = m_factory((details) => () => details.createOnigurumaEngine(import('shiki/wasm')));

    //  PUBLIC METHODS  //

    /**
     * Handles rendering code-blocks.
     * @param code              Code to highlight.
     * @param options           Highlight options.
     */
    export async function render(code: string, { components, ...options }: Options = {}) {
        // resolve a suitable engine to be used
        const engine = options.engine ?? 'wasm';
        const factory = engine === 'js' ? m_js : m_wasm;
        const highlighter = await factory.getOrInit();

        // resolve the options to be used now
        const resolved = m_resolve(options);

        // attempt highlighting the language now
        await m_missing(highlighter, resolved.lang);
        const root = highlighter.codeToHast(code, resolved);

        // and finally convert to our desired runtime
        return hast.toJsxRuntime(root, { ...react, development: false, components });
    }

    //  PRIVATE METHODS  //

    /**
     * Handles resolving highlighting options.
     * @param options                   Options to resolve.
     */
    function m_resolve({ lang = 'text', ...options }: Omit<Options, 'engine' | 'components'>): shiki.CodeToHastOptions {
        (options.transformers ??= []).push(Transformer.Deprecated());
        return { lang, defaultColor: false, themes: Themes, ...options };
    }

    /**
     * Handles resolving missing languages.
     * @param highlighter               Highlighter instance.
     * @param langs                     Languages to resolve.
     */
    async function m_missing(higlighter: shiki.HighlighterCore, ...langs: string[]) {
        // prepare the bundled set of languages
        const bundled = higlighter.getBundledLanguages();

        // and attempt loading any that are missing now
        for (const lang of langs) {
            try {
                if (lang in bundled) higlighter.getLanguage(lang);
            } catch {
                await higlighter.loadLanguage(lang as never);
            }
        }
    }

    /**
     * Handles constructing factories.
     * @param engine                Engine builder.
     */
    function m_factory(engine: (details: typeof shiki) => () => Promise<shiki.RegexEngine>) {
        return createShikiFactory({
            async init(options) {
                const details = await import('shiki');
                return details.createHighlighter({
                    langs: Grammar(),
                    engine: engine(details)(),
                    langAlias: options?.langAlias,
                    themes: Array.from(Object.values(Themes)),
                });
            },
        });
    }
}
