/// Node Modules
import * as path from 'node:path';

/// Vendor Modules
import { File, Files, Folder } from 'fumadocs-ui/components/files';

/// Package Modules
import { Icon as _Icon } from '../icon';

/** File Tree Component. */
export interface Tree extends Tree.Props {}
export function Tree(props: Tree) {
    return <Files {...props} />;
}

export namespace Tree {
    //  TYPEDEFS  //

    /** Tree Component Properties. */
    export type Props = React.ComponentProps<typeof Files>;

    //  PROPERTIES  //

    /** Available pre-built icons. */
    const m_icons: Record<string, _Icon> = {
        '.sabre': 'sword',
        '.json': 'braces',
        '.jsonc': 'braces',
    };

    //  PUBLIC METHODS  //

    /** Tree Leaf Component. */
    export type Leaf = Omit<React.ComponentProps<typeof File>, 'icon'> & { icon?: _Icon };
    export function Leaf({ icon, ...props }: Leaf) {
        return <File {...props} icon={Icon({ icon, ...props })} />;
    }

    /** Tree Branch Component. */
    export type Branch = React.ComponentProps<typeof Branch>;
    export const Branch = Folder;

    //  PRIVATE METHODS  //

    /** Constructs a suitable file-icon. */
    export function Icon({ icon, name }: Leaf) {
        // handle with the base incoming icon details
        if (typeof icon !== 'undefined') return _Icon.Dynamic(icon as any);

        // otherwise handle based on the incoming extension now
        return _Icon.Dynamic(m_icons[path.extname(name)] as any);
    }
}
