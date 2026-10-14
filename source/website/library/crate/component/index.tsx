/// Vendor Modules
import { Heading } from 'fumadocs-ui/components/heading';

/// Website Modules
import { Docify } from '@/website/docify';
import { Code, Fragment } from '@/website/components';

/// Package Modules
import { Registry } from '../registry';
import { Manifest } from '../manifest';

/** Crate Documentation Component. */
export interface Component extends Component.Props {}
export function Component({ name, children }: Component) {
    // attempt resolving the current manifest
    const manifest = Registry.resolve(name);
    if (typeof manifest === 'undefined') return;

    // if we have no outputs, show the user some custom content
    const empty = typeof manifest.project === 'undefined' || manifest.project.groups.length === 0;
    if (empty) children = <p>This crate is current unimplemented and its details are pending.</p>;

    // and return the resulting documentation
    return (
        <Fragment>
            <Heading as="h2" id="overview" children="Overview" />
            {children ?? <p>Unimplemented...</p>}
            <Component.Project project={manifest.project} />
            <p>{/* This is some custom padding here */}</p>
        </Fragment>
    );
}

export namespace Component {
    //  TYPEDEFS  //

    /** Documentation Component Properties. */
    export type Props = React.PropsWithChildren<{ readonly name: string }>;

    //  PUBLIC METHODS  //

    /**
     * Constructs a suitable project output.
     * @param manifest                  Minimal manifest details.
     */
    export function Project({ project }: Pick<Manifest, 'project'>) {
        // ignore when not suitably given a good value
        if (typeof project === 'undefined' || project.groups.length === 0) return;

        // attempt converting all our groups now
        return project.groups.flatMap((group, ii) => [
            <Heading key={`${group.title}-${ii}`} as="h2" id={group.title.toLowerCase()} children={group.title} />,
            ...group.children.flatMap((identifier, jj) => {
                // prepare the output node and comments
                const node = project.nodes[identifier];

                // prepare the type-definition as necessary
                const code = m_comments(node)
                    .concat([m_typedef(node)])
                    .join('\n');

                // prepare a suitable identifier to be used
                const id = `${group.title.toLowerCase()}-${node.name.toLowerCase()}`;

                // and merge our details back together here
                return [
                    <Heading key={`${node.name}-${jj}`} as="h3" id={id} children={<code>{node.name}</code>} />,
                    <Code.Dynamic key={`${node.name}-${jj}-code`} lang="sabre" code={code} />,
                ];
            }),
        ]);
    }

    //  PRIVATE METHODS  //

    /**
     * Constructs outgoing comments.
     * @param node              Node to resolve.
     */
    function m_comments(node: Docify.Node): string[] {
        if (typeof node.description === 'undefined') return [];
        return node.description.split('\n').map((line) => `//! ${line}`);
    }

    /**
     * Constructs outgoing type definitions.
     * @param node              Node to resolve.
     */
    function m_typedef(node: Docify.Node): string {
        // prepare the transforms to be used
        const transforms = {
            [Docify.Variant.ENUM]: `enum ${node.typing} { ... }`,
            [Docify.Variant.CLASS]: `${node.typing} { ... }`,
            [Docify.Variant.NAMESPACE]: `namespace ${node.typing} { ... }`,
            [Docify.Variant.TYPEDEF]: `type ${node.name} = ${node.typing}`,
            [Docify.Variant.VARIABLE]: `let ${node.name}: ${node.typing}`,
        } satisfies Record<Docify.Variant, string>;

        // and resolve a suitable output set of details now
        return node.variant ? transforms[node.variant] : node.typing;
    }
}
