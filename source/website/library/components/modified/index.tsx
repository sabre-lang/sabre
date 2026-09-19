/// Vendor Modules
import { getGithubLastEdit, GetGithubLastCommitOptions } from 'fumadocs-core/content/github';

/// Website Modules
import { cn } from '@/website/utilities';
import { Product } from '@/website/product';

/** Gets the last modified time for markdown pages. */
export interface Modified extends Modified.Props {}
export async function Modified({ path, options = {}, className, ...props }: Modified) {
    // revise the outgoing options to be used and get the necessary last-modified details
    const input = { path: `docs/${path}`, owner: 'sabre-lang', repo: 'sabre', ...options };
    const edited = Product.development ? null : await getGithubLastEdit(input).catch(() => null);

    // prepare a suitable format to be used as well
    const format: Intl.DateTimeFormatOptions = { month: 'long', day: '2-digit', year: 'numeric' };

    // and construct a suitable last-updated timestamp to be used
    return (
        edited && (
            <div className={cn('text-fd-muted-foreground border-t mt-5 py-5', className)} {...props}>
                Last updated on {edited.toLocaleString('default', format)}
            </div>
        )
    );
}

export namespace Modified {
    //  TYPEDEFS  //

    /** Last Modified Component Properties. */
    export type Props = Omit<React.JSX.IntrinsicElements['div'], 'children'> & {
        path: string;
        options?: Omit<Partial<GetGithubLastCommitOptions>, 'path'>;
    };
}
