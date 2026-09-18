/// Vendor Modules
import * as vscode from 'vscode';

/// Sabre Modules
import { Product } from '@/vscode/product';

/** Output Channel Factory. */
export interface Channel extends vscode.LogOutputChannel {}
export const Channel = (label?: string): Channel =>
    vscode.window.createOutputChannel(`${Product.shortName}${label ? ' - ' : ''}${label ?? ''}`, { log: true });
