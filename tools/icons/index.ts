/// Node Modules
import * as fs from 'node:fs';

/// Vendor Modules
import sharp from 'sharp';

/// Tools Modules
import { Assets } from '~/tools/assets/index';

//  TOOL RUNNER  //

(async () => {
    // prepare some common directories to be used
    const outdir = Assets.vscode('icons');

    // prepare the image directories to be used
    const svg = Assets.root('icons', 'sabre.svg');
    const png = Assets.resolve(outdir, 'sabre.png');

    const buffer = await sharp(svg).png().resize(128, 128).toBuffer();
    if (!fs.existsSync(outdir)) await fs.promises.mkdir(outdir, { recursive: true });
    await fs.promises.writeFile(png, buffer); // and write the output
    await fs.promises.copyFile(svg, Assets.resolve(outdir, 'sabre.svg'));
})();
