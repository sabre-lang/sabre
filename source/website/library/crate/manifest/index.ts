/// Website Modules
import { Docify } from '@/website/docify';

/** Crate Manifest Details. */
export interface Manifest {
    readonly name: string;
    readonly version: string;
    readonly describe?: string;
    readonly resource: string;
    readonly project?: Docify.Project;

    /** Handles resolving crate paths. */
    resolve(...segments: string[]): string;
}
