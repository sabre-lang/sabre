/// Node Modules
import * as path from 'node:path';

/** Internal Asset Paths. */
export type Assets = typeof path.resolve;
export namespace Assets {
    //  PROPERTIES  //

    const m_project = /* turbopackIgnore: true */ process.cwd();
    const m_root = path.resolve(/* turbopackIgnore: true */ m_project, '..', '..');
    const m_docs = path.resolve(/* turbopackIgnore: true */ m_root, 'docs');
    const m_blog = path.resolve(/* turbopackIgnore: true */ m_docs, 'blog');
    const m_scripts = path.resolve(/* turbopackIgnore: true */ m_root, 'scripts');
    const m_source = path.resolve(/* turbopackIgnore: true */ m_root, 'source');
    const m_examples = path.resolve(/* turbopackIgnore: true */ m_root, 'examples');
    const m_syntaxes = path.resolve(/* turbopackIgnore: true */ m_root, 'syntaxes');
    const m_crates = path.resolve(/* turbopackIgnore: true */ m_source, 'crates');
    const m_builtins = path.resolve(/* turbopackIgnore: true */ m_source, 'sabre', 'builtins');

    //  PUBLIC METHODS  //

    /** The project directory. */
    export const project = m_bind(m_project);

    /** The underlying root directory. */
    export const root = m_bind(m_root);

    /** The underlying scripts director. */
    export const scripts = m_bind(m_scripts);

    /** The available language configuration. */
    export const syntaxes = m_bind(m_syntaxes);

    /** The documentation folder. */
    export const docs = m_bind(m_docs);

    /** The blogs folder. */
    export const blog = m_bind(m_blog);

    /** The underlying examples path. */
    export const examples = m_bind(m_examples);

    /** The underlying crates path. */
    export const crates = m_bind(m_crates);

    /** The underlying builtins path. */
    export const builtins = m_bind(m_builtins);

    //  PRIVATE METHODS  //

    /**
     * Handles binding asset resolvers.
     * @param base              Base path.
     */
    function m_bind(base: string | Assets): Assets {
        if (typeof base === 'function') return base; // should be a normal passthrough
        return (...segments) => (segments.length ? path.resolve(base, ...segments) : base);
    }
}
