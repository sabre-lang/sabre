/// Declare Client
'use client';

/// CSS Modules
import './style.css';

/// Vendor Modules
import dynamic from 'next/dynamic';
import { useEffect, useState } from 'react';
import type { GrainGradientProps } from '@paper-design/shaders-react';

/// Website Modules
import { cn } from '@/website/utilities';
import { Product } from '@/website/product';

/// Package Modules
import { Button } from '../button';

/** Constructs a hero-component. */
export interface Hero extends Hero.Props {}
export function Hero({ className, ...props }: Hero) {
    // prepare some state to be used here
    const [showShaders, setShowShaders] = useState(false);

    // allow enabling the shaders after a small amount of time
    useEffect(() => void setTimeout(() => setShowShaders(true), 100), []);

    // prepare the className to be used
    const height = 'h-[200px] sm:h-[310px]';
    const grid = 'grid grid-cols-8 justify-center items-center';
    className = cn(className, height, grid, 'relative border rounded-2xl overflow-hidden bg-origin-border');

    // prepare the dithering options to be used
    const grain: GrainGradientProps = {
        colorBack: '#00000000',
        colors: ['#AA6C39FF'],
        className: 'absolute hero-animation w-full h-full z-0',
    };

    // prepare the hero actions available now
    const actions = [
        <Button key="welcome" href="/language" title={'Learn'} icon="book-text" />,
        <Button key="github" variant="secondary" href={Product.github} title={'GitHub'} icon="git-branch" />,
    ];

    // and construct the home-page hero poster
    return (
        <div {...props} className={className}>
            <div className="flex flex-col col-span-8 sm:col-span-5 px-6 py-8 z-10 sm:h-full">
                <h1 className="text-4xl sm:text-5xl mb-1! sm:mb-2 font-mono italic">{Product.shortName}</h1>
                <h2 className="text-2xl block sm:hidden font-mono my-0!">v{Product.version}</h2>
                <h2 className="text-3xl hidden sm:block">{Product.description}</h2>
            </div>

            <div className="flex col-span-8 sm:col-span-3 items-end justify-end px-6 pb-8 z-10 sm:h-full gap-2">
                {actions}
            </div>

            {showShaders && <Hero.Gradient {...grain} />}
        </div>
    );
}

export namespace Hero {
    //  TYPEDEFS  //

    /** Hero Properties Available. */
    export type Props = React.HTMLAttributes<HTMLElement>;

    //  PUBLIC METHODS  //

    /// The available gradient module
    export interface Gradient extends GrainGradientProps {}
    export const Gradient = dynamic(
        () => import('@paper-design/shaders-react').then(({ GrainGradient }) => GrainGradient),
        { ssr: true }, // set some additional loading properties as well
    );
}
