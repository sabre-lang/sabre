/// Node Modules
import * as fs from 'node:fs';

/// Vendor Modules
import * as YAML from 'yaml';

/// Tool Modules
import { Assets } from '~/tools/assets/index';

//  TOOL RUNNER  //

(async () => {
    // prepare some common directories to be used
    const dst = Assets.vscode('syntaxes');

    // prepare the names of the items to move
    const json = 'sabre.tmLanguage.json';
    const yaml = 'sabre.tmLanguage.yaml';
    const inject = 'sabre.injection.json';

    // get the underlying content to be emitted
    const content = YAML.parse(await fs.promises.readFile(Assets.syntaxes(yaml), 'utf-8'));

    // ensure the destination actually exists now
    if (!fs.existsSync(dst)) await fs.promises.mkdir(dst, { recursive: true });

    // and write the resulting content now
    await fs.promises.writeFile(Assets.resolve(dst, json), JSON.stringify(content, undefined, 4));

    // also copy the injection syntax to be used as well
    await fs.promises.copyFile(Assets.syntaxes(inject), Assets.resolve(dst, inject));
})();
