/// Node Modules
import * as path from 'node:path';
import * as cp from 'node:child_process';

/// Tools Modules
import { Assets } from '~/tools/assets/index';

//  TOOL RUNNER  //

(async () => {
    // prepare some paths to be used now
    const outdir = path.resolve(__dirname);
    const prefix = path.resolve(outdir, `callgrind.out.`);
    const script = Assets.examples('sandbox.sabre');
    const executable = Assets.root('dist', 'bin', 'sabre');
    const argv = process.argv.slice(2).reduce((acc, argn) => acc + `${argn} `, '');
    const visualizer = `${process.platform === 'linux' ? 'k' : 'q'}cachegrind`;

    const options: cp.ExecSyncOptions = { cwd: outdir, stdio: 'inherit' };
    const valgrind = `valgrind --tool=callgrind --callgrind-out-file=${prefix}raw ${executable} run ${argv}${script}`;
    const annotate = `callgrind_annotate --tree=both --inclusive=yes --auto=yes --show-percs=yes ${prefix}raw > ${prefix}annotated`;

    // await running the tool now as necessary
    cp.execSync(valgrind, options);
    cp.execSync(annotate, options);

    // attempt visualizing as necessary
    cp.execSync(`${visualizer} ${prefix}raw`, options);
})();
