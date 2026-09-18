/// Node Modules
import * as fs from 'node:fs';

/// Vendor Modules
import * as YAML from 'yaml';
import type * as shiki from 'shiki';

/// Package Modules
import { Assets } from '../assets';
import { Product } from '../product';

//  PROPERTIES  //

/** Cached grammar definition. */
let g_grammar: Grammar | undefined = undefined;

//  PUBLIC METHODS  //

/** Handles getting the underlying grammar. */
export type Grammar = shiki.LanguageInput;
export function Grammar(): Grammar {
    try {
        return (g_grammar ??= YAML.parse(fs.readFileSync(Assets.syntaxes('sabre.tmLanguage.yaml'), 'utf-8')));
    } catch {
        return { patterns: [], repository: {}, name: Product.identifier, scopeName: `source${Product.extension}` };
    }
}
