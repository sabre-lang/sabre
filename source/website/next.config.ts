/// Node Modules
import * as cp from 'node:child_process';

/// Vendor Modules
import * as fumadocs from 'fumadocs-mdx/next';

/// JSON Modules
import PKG from '~/package.json';

//  ENVIRONMENT VARIABLES  //

/** The latest release version. */
const SABRE_VERSION = m_version();

//  CONFIGURATION EXPORTS  //

/** Next JS Configuration */
export default fumadocs.createMDX()({
    output: 'export',
    reactStrictMode: true,
    env: { SABRE_VERSION },
});

//  PRIVATE METHODS  //

function m_version() {
    // prepare the command to be used for getting current release version
    const command = 'gh release view --repo sabre-lang/sabre --json tagName --jq .tagName';

    try {
        const output = cp.execSync(command, { stdio: 'pipe' });
        return output.toString().split('-')[0].trim();
    } catch (error) {
        return `v${PKG.version}`;
    }
}
