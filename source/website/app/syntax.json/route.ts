/// Node Modules
import * as fs from 'node:fs';

/// Vendor Modules
import * as YAML from 'yaml';
import { NextResponse } from 'next/server';

/// Website Modules
import { Assets } from '@/website/assets';

/** Ensure no caching occurs. */
export const revalidate = false;

/** Gets the "install.sh" script. */
export async function GET() {
    const body = await fs.promises.readFile(Assets.syntaxes('sabre.tmLanguage.yaml'), 'utf-8'); // resolve now
    const headers = { 'Content-Disposition': 'inline; filename="syntax.json"', 'Content-Type': 'application/json' };
    return new NextResponse(JSON.stringify(YAML.parse(body), undefined, 4), { headers }); // and construct now
}
