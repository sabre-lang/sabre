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
export function Grammar(): Grammar[] {
    return [Grammar.Sabre(), Grammar.Colorless()];
}

export namespace Grammar {
    //  PUBLIC METHODS  //

    /** Resolves the Colorless grammar. */
    export function Colorless(): Grammar {
        return {
            name: 'colorless',
            scopeName: 'source.colorless',
            patterns: [{ include: '#comments' }, { include: '#blue' }, { include: '#red' }],
            repository: {
                red: { match: '(red_function|async|await)', name: 'keyword.other.red' },
                blue: { match: '(blue_function|function)', name: 'constant.numeric.blue' },
                comments: { match: '((//).*)$', captures: { 1: { name: 'comment.line' } } },
            },
        };
    }

    /** Resolves the Sabre grammar. */
    export function Sabre(): Grammar {
        try {
            return (g_grammar ??= YAML.parse(fs.readFileSync(Assets.syntaxes('sabre.tmLanguage.yaml'), 'utf-8')));
        } catch {
            return { patterns: [], repository: {}, name: Product.identifier, scopeName: `source${Product.extension}` };
        }
    }
}
