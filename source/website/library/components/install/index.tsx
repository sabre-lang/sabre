/// Vendor Modules
import Link from 'next/link';
import { Eye } from 'lucide-react';
import { buttonVariants } from 'fumadocs-ui/components/ui/button';
import { Tab, Tabs, TabsList, TabsTrigger } from 'fumadocs-ui/components/tabs';

/// Website Modules
import { cn } from '@/website/utilities';

/// Package Modules
import { Code } from '../code';
import { Fragment } from '../fragment';

/** Install Component Factory. */
export interface Install extends Install.Props {}
export function Install({}: Install) {
    // prepare the scripts to be shown now as well
    const scripts: Install.Script[] = [
        {
            lang: 'bash',
            href: '/install.sh',
            title: 'Linux / Darwin',
            content: 'curl -fsSL https://sabre.rroessler.io/install.sh | bash',
        },
        {
            lang: 'powershell',
            href: '/install.ps1',
            title: 'Windows',
            content: 'powershell -c "irm https://sabre.rroessler.io/install.ps1 | iex"',
        },
    ];

    // prepare the available tabs and triggers
    const initial = scripts[0].lang;
    const tabs = scripts.map((props, ii) => <Install.Script key={ii} {...props} />);
    const triggers = scripts.map((props, ii) => <Install.Trigger key={ii} {...props} />);

    // push the custom triggers for viewing the current script
    triggers.push(...scripts.map((props, ii) => <Install.Viewer key={ii + scripts.length} {...props} />));

    // prepare the resulting children to be shown now
    const children = [
        <TabsList key="list" children={triggers} />,
        tabs.map((tab, ii) => <Fragment key={ii} children={tab} />),
    ];

    // prepare the resulting tabs now
    return <Tabs defaultValue={initial} children={children} />;
}

export namespace Install {
    //  TYPEDEFS  //

    /** Install Component Properties. */
    export type Props = Omit<React.HTMLAttributes<HTMLElement>, 'children'>;

    /** Baseline script details. */
    export interface Script {
        lang: string;
        href: string;
        title: string;
        content: string;
    }

    //  PROPERTIES  //

    /** Prepare the peers class-names. */
    const m_peers: Record<string, string> = {
        bash: 'peer/bash',
        powershell: 'peer/powershell',
    };

    /** Prepare the variants class-names. */
    const m_variants: Record<string, string> = {
        bash: 'peer-data-[state=inactive]/bash:hidden',
        powershell: 'peer-data-[state=inactive]/powershell:hidden',
    };

    //  PUBLIC METHODS  //

    /** Allows viewing scripts. */
    export function Viewer({ href, lang }: Script) {
        // prepare the baseline classes and variant details
        const className = `text-fd-muted-foreground hover:text-fd-accent-color ms-auto`;
        const variant = buttonVariants({ className: cn(className, m_variants[lang]), size: 'icon-xs' });

        // we then need to define our link so it does not download automatically
        return (
            <Link
                href={href}
                target="_blank"
                type="button"
                rel="noopener noreferrer"
                className={variant}
                children={<Eye />}
            />
        );
    }

    /** Available Script Triggers. */
    export function Trigger({ title, lang }: Script) {
        return <TabsTrigger value={lang} className={m_peers[lang]} children={title} />;
    }

    /** Shows the install script. */
    export function Script({ content, lang }: Script) {
        return <Tab value={lang} children={<Code.Dynamic lang={lang} code={content} />} />;
    }
}
