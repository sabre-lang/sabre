/// Vendor Modules
import * as React from 'react';

/// Website Modules
import { cn } from '@/website/utilities';

/** Callout Component. */
export interface Callout extends Callout.Props {}
export function Callout({ children, type, title, className, style, ...props }: Callout) {
    // prepare the incoming class-name to be used
    className = cn('flex gap-2 my-2 p-3 ps-1 text-sm border-l-0 text-fd-card-foreground', className);

    // update the current styling as well (need to force the use of our desired callout values through `any`)
    style = { '--callout-color': `var(--color-fd-${type ?? 'info'}, var(--color-fd-muted))`, ...style } as any;

    // update the children to prepare a title if necessary to do so
    children = Callout.Title(children, title);

    // construct the container of the callout now
    return (
        <div className={className} style={style} {...props}>
            <div role="none" className="w-0.75 bg-(--callout-color)/50 rounded-sm" />
            <div className="text-fd-muted-foreground prose-no-margin empty:hidden min-w-0" children={children} />
        </div>
    );
}

export namespace Callout {
    //  TYPEDEFS  //

    /** Available Callout Types. */
    export type Type = 'info' | 'warning' | 'error' | 'success' | 'idea';

    /** Button Component Properties. */
    export interface Props extends Omit<React.ComponentProps<'div'>, 'title'> {
        type?: Type;
        title?: React.ReactNode;
    }

    //  PUBLIC METHODS  //

    /**
     * Helper to bind titles to callouts.
     * @param children              Children given.
     * @param title                 Optional title.
     */
    export function Title(children: React.ReactNode, title?: React.ReactNode) {
        // ignore if not given an initial title
        if (typeof title === 'undefined') return children;

        // prepare the title prefix to be used
        const prefix = (
            <strong key="callout-title" className="text-fd my-0!" style={{ color: 'var(--tw-prose-bold)' }}>
                {title}:&nbsp;
            </strong>
        );

        // map the children to update the title value now
        children = React.Children.map(children, (child: any) => {
            if (typeof title === 'undefined') return child;
            if (!React.isValidElement(child) || child.type !== 'p') return child;

            // ensure we latch the title now
            title = undefined;

            // can safely clone our element with our required title
            return React.cloneElement(child, child.props as any, prefix, (child.props as any).children);
        });

        // check if the title has been applied at all
        if (typeof title === 'undefined') return children;

        // otherwise, we just concatenate the two values
        return [prefix, children];
    }
}
