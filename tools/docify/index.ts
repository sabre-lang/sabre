/// Node Modules
import * as fs from 'node:fs';
import * as path from 'node:path';
import * as cp from 'node:child_process';

/// Tools Modules
import { Assets } from '~/tools/assets/index';

//  TOOL RUNNER  //

(async () => {
    // prepare some paths to be used now
    const prefix = Assets.crates();

    // determine a suitable executable to be used
    const executable = (() => {
        const extension = process.platform === 'win32' ? '.exe' : '';
        const initial = Assets.root('dist', 'bin', `sabre${extension}`);
        if (fs.existsSync(initial)) return initial; // use internal value
        const search = process.platform === 'win32' ? 'where' : 'which';
        return cp.execSync(`${search} sabre`).toString().trim(); // scan
    })();

    // attempt resolving all the suitable crate files now
    const filter = (name: string) => !name.startsWith('_') && fs.existsSync(path.join(prefix, name, '_crate.jsonc'));
    const crates = await fs.promises.readdir(prefix).then((names) => names.filter(filter));

    // attempt emitting all the documentation required for the crates
    const promises = crates.map(
        (crate) =>
            new Promise<void>((resolve, reject) => {
                const outfile = path.join(prefix, crate, '_docify.jsonc'); // prepare the output file
                const child = cp.spawn(executable, ['docify', '--outfile', outfile, `sabre:${crate}`]);
                (child.on('exit', resolve), child.on('error', reject)); // bind some suitable events
            }),
    );

    // finally wait for all the promises to settle (don't care about results yet)
    await Promise.allSettled(promises);
})().catch((error) => console.warn(error));
