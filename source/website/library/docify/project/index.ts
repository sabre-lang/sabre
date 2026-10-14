/** Variant Enumeration. */
export const enum Variant {
    TYPEDEF = 'typedef',
    VARIABLE = 'variable',

    ENUM = 'enum',
    CLASS = 'class',
    NAMESPACE = 'namespace',
}

/** Group Interface. */
export interface Group {
    readonly title: string;
    readonly description?: string;
    readonly children: string[];
}

/** Declaration Interface. */
export interface Node {
    readonly name: string;
    readonly description?: string;
    readonly typing: string;
    readonly variant?: Variant;
}

/** Project Interface. */
export interface Project {
    readonly title: string;
    readonly groups: Group[];
    readonly description: string;
    readonly nodes: Record<string, Node>;
}
