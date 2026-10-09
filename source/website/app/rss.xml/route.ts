/// Vendor Modules
import { Feed } from 'feed';

/// Website Modules
import { Source } from '@/website/source';
import { Product } from '@/website/product';

/** Force Disabling Revalidation. */
export const revalidate = false;

/** Allows getting the current RSS feed details. */
export function GET() {
    // prepare the base resource to be used
    const resource = 'https://sabre.rroessler.io';

    // construct the internal feed value now
    const feed = new Feed({
        language: 'en',
        id: `${resource}/blog`,
        link: `${resource}/blog`,
        description: Product.description,
        title: `${Product.shortName} Blog`,
        copyright: 'All rights reserved 2026, Reuben Roessler',
    });

    // fill out all the available blog pages now
    for (const page of Source.blog.getPages()) {
        // ignore any potential draft pages
        if (page.data.draft) continue;

        // append the feed item now
        feed.addItem({
            id: page.url,
            title: page.data.title,
            description: page.data.description,
            link: `${resource}${page.url}`,
            date: new Date(page.data.date),
            author: [{ name: 'Reuben Roessler' }],
        });
    }

    // and expose the response now
    return new Response(feed.rss2());
}
