/// Website Modules
import { cn } from '@/website/utilities';
import { Product } from '@/website/product';
import { Portal } from '@/website/components';

/// Package Modules
import { Registry } from '../registry';

/** Crate Overview Component. */
export interface Overview extends Omit<React.ComponentProps<'div'>, 'children'> {}
export function Overview({ className, ...props }: Overview) {
    // prepare all the available portals to be show now
    const portals = Registry.list().map(({ name, describe }) => {
        const base = name.slice(Product.identifier.length + 2); // ensure we get the base-name now to be used
        return <Portal key={name} name={`sabre:${base}`} href={`/crates/${base}`} description={describe} />;
    });

    // revise the incoming class-name to be used
    className = cn(className, 'mt-8 grid grid-cols-1 gap-4 text-start md:grid-cols-2 w-full');

    // and construct the resulting output
    return <div {...props} className={className} children={portals} />;
}
