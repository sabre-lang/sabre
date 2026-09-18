/// Vendor Modules
import type { ShikiTransformer } from 'shiki';
import type { Element, ElementContent, Text } from 'hast';
import type { ShikiTransformerContext } from 'shiki/core';
import { createCommentNotationTransformer } from '@shikijs/transformers';

/** Available Shiki Transformers. */
export type Transformer = ShikiTransformer;
export namespace Transformer {
    //  PROPERTIES  //

    const m_unescape = /\\(.)/g;
    const m_deprecated = /\s*\[!code deprecated:((?:\\.|[^:\]])+)(:\d+)?\]/;

    //  PUBLIC METHODS  //

    /** Strikes Deprecated Words. */
    export function Deprecated(): Transformer {
        return createCommentNotationTransformer(
            '@sabre/transformers:notation-deprecated-word',
            m_deprecated,
            function ([, word, range], _, comment, lines, index) {
                const ln = range ? Number.parseInt(range.slice(1), 10) : lines.length;
                word = word.replace(m_unescape, '$1'); // unescape the incoming word now

                // and highlight all the found words now
                for (let ii = index; ii < Math.min(index + ln, lines.length); ++ii) {
                    m_highlight.call(this, lines[ii], comment, word, 'highlighted-deprecated');
                }

                // and return the final result now
                return true;
            },
            undefined,
        );
    }

    //  PRIVATE METHODS  //

    function m_highlight(
        this: ShikiTransformerContext,
        line: Element,
        ignoredElement: Element | null,
        word: string,
        className: string,
    ): void {
        const content = m_content(line);
        let index = content.indexOf(word);

        while (index !== -1) {
            m_range.call(this, line.children, ignoredElement, index, word.length, className);
            index = content.indexOf(word, index + 1);
        }
    }

    function m_content(element: ElementContent): string {
        if (element.type === 'text') return element.value;
        if (element.type !== 'element' || element.tagName !== 'span') return '';
        return element.children.map(m_content).join(''); // can safely join now
    }

    function m_range(
        this: ShikiTransformerContext,
        elements: ElementContent[],
        ignoredElement: Element | null,
        index: number,
        len: number,
        className: string,
    ): void {
        let currentIdx = 0;

        for (let i = 0; i < elements.length; i++) {
            const element = elements[i];
            if (element.type !== 'element' || element.tagName !== 'span' || element === ignoredElement) continue;
            const textNode = element.children[0];
            if (textNode.type !== 'text') continue;

            // check if it is overlapped with highlight range
            if (m_overlap([currentIdx, currentIdx + textNode.value.length - 1], [index, index + len])) {
                const start = Math.max(0, index - currentIdx);
                const length = len - Math.max(0, currentIdx - index);
                if (length === 0) continue; // ignore if invalid length

                const separated = m_seperate(element, textNode, start, length);
                this.addClassToHast(separated[1], className); // bind now

                // insert the seperated items now
                const output = separated.filter(Boolean) as Element[];

                // and splice our details to be used now
                elements.splice(i, 1, ...output);
                i += output.length - 1;
            }

            currentIdx += textNode.value.length;
        }
    }

    function m_overlap(range1: [number, number], range2: [number, number]): boolean {
        return range1[0] <= range2[1] && range1[1] >= range2[0];
    }

    function m_seperate(
        span: Element,
        textNode: Text,
        index: number,
        len: number,
    ): [before: Element | undefined, med: Element, after: Element | undefined] {
        const text = textNode.value; // prepare the text and builder to be used now
        const builder = (value: string): Element => m_inherit(span, { children: [{ type: 'text', value }] });

        return [
            index > 0 ? builder(text.slice(0, index)) : undefined,
            builder(text.slice(index, index + len)),
            index + len < text.length ? builder(text.slice(index + len)) : undefined,
        ];
    }

    function m_inherit(original: Element, overrides: Partial<Element>): Element {
        return { ...original, properties: { ...original.properties }, ...overrides };
    }
}
