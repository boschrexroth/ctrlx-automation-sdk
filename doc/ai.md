# Using AI Agents with the ctrlX AUTOMATION SDK

The ctrlX AUTOMATION SDK supports popular AI coding agents. It includes persistent instructions (`AGENTS.md`), skills (`SKILLS.md`), and workflows that can be used by popular agents such as [GitHub Copilot](https://github.com/copilot), [Claude Code](https://claude.com/de/product/claude-code), [Cursor](https://cursor.com/), and [OpenAI Codex](https://openai.com/de-DE/codex/).

Use your preferred AI agent to explore SDK samples, generate application code, understand Data Layer APIs, and build or troubleshoot your ctrlX app.


1. Open the SDK folder in your IDE or terminal.
2. Start the Chat and describe your goal, including the target language and platform.
3. Refer to an existing sample in a `samples-*` folder.
4. Ask the agent to adapt the sample, then build and test the result using the SDK-provided skills.

Example prompt:

    Create a C++ ctrlX Data Layer client based on the SDK samples.
    Read a value from the device and provide the required CMake and Snap packaging files.

**IMPORTANT:**

Always review AI-generated code, test it on the target system, and ensure that configuration, credentials, and certificates are handled securely.

This introduction is based on [GitHub Copilot CLI](https://github.com/github/copilot-cli). Other AI agents may have similar usage patterns.

## GitHub Copilot CLI

You need a [GitHub](https://github.com/) account. Visit [GitHub Copilot settings](https://github.com/settings/copilot) to subscribe or manage your access. Configuring [two-factor authentication](https://github.com/settings/security) is strongly recommended.

### Setup your host machine (Windows)

1. Ensure that the following software and services are available:

    + ctrlX WORKS is installed. It provides the App Build Environment (QEMU VM).
    + The [App Build Environment (QEMU VM)](setup_qemu_ctrlx_works.md) has been created and started.
    + [VS Code](https://code.visualstudio.com/) and the [GitHub Copilot](https://marketplace.visualstudio.com/items?itemName=GitHub.copilot) extension are installed.

2. Open VS Code.

    + Click the *Copilot* icon in the lower-right corner, or open the Command Palette with <Ctrl+Shift+P>.
    + Select *Sign in to GitHub* and log in with your GitHub account.
    + If you don't have a paid Copilot subscription, GitHub should offer Copilot Free automatically.

3. Ensure that the agent has internet connectivity.

    > !!! info If your Windows host is not behind a **Corporate proxy**, skip this step.

    + Download [Px Proxy](https://github.com/genotrance/px/releases) (select **Show all assets** and download latest `px-v*windows-amd64.zip`), then extract it.

    + Configure Px Proxy as a [Windows service](https://github.com/genotrance/px/blob/master/docs/installation.md#running-as-a-windows-service-using-winsw).

    + Install Px Proxy as a Windows service:

        `WinSW-x64.exe install WinSW-x64.xml`

    + **Remark:** Remove the Px Proxy Windows service when it is no longer needed:

        `WinSW-x64.exe uninstall WinSW-x64.xml`

### Install on App Build Environment (QEMU VM)

1. Start and set up the [App Build Environment](setup_qemu_ctrlx_works.md) properly.
2. Install *Node.js* by running `source ./scripts/install-nodejs-npm.sh` in the terminal.

3. Install GitHub Copilot CLI.

    + Start VS Code and log in to the App Build Environment (QEMU VM) through SSH.
    + Install [GitHub Copilot CLI](https://github.com/github/copilot-cli) by running `npm install -g @github/copilot` in the terminal. See also the [Copilot CLI quickstart](https://docs.github.com/en/copilot/get-started/cli-quickstart).

4. Start and log in through the terminal.

    + Start Copilot CLI with `copilot`.
    + Log in with `/login` and follow the authentication steps.
    + Optionally choose or change to your preferred LLM Model by typing `/model`.

You are now ready to use GitHub Copilot CLI. Press `/` to see all available commands, or just enter a prompt.

For more information, see the [GitHub Copilot documentation](https://docs.github.com/en/copilot).

## Built-in Skills
The SDK comes with a rich set of useful `SKILLS.md`. See folder `.github/skills` for all available ones.
All Skills are added to the CLI command list automatically and can be browsed by typing `/`.

Example Usage:
+ Navigate the Agent to any SDK sample by using the *internal* `/cd` command or start the Agent directly from the current directory if your terminal was already started from there.
+ Start typing `/` and choose `/build-snap-amd64`, `/explain-code`, `/security-review`, ...

<a id="install-pxproxy"></a>
### Skill: Install Px Proxy
Name: `/install-pxproxy`
Use the skill to configure [Px Proxy](https://github.com/genotrance/px) on the local computer for the App Build Environment (QEMU VM).

**Usage**
+ Download the skill [Skills](https://github.com/boschrexroth/ctrlx-automation-sdk/tree/main/.github/.github/skills) to `/.github/skills` folder on your **local machine**, refresh the skills with `/skill reload`.
+ Run with `/install-pxproxy` on your **local machine**.

<a id="setup-ssh"></a>
### Skill: Setup SSH access for App Build Environment (QEMU VM)
Name: `/setup-ssh`
Use the skill to setup SSH with public key authentication for the App Build Environment (QEMU VM) after initial startup.

**Prerequisites:**
+ [VSCode ](https://code.visualstudio.com/) installed.
+ Px Proxy is installed and running (use skill [install-pxproxy](#install-pxproxy)).
+ App Build Environment (QEMU VM) is created and running.

**Usage**
+ Download the skill [Skills](https://github.com/boschrexroth/ctrlx-automation-sdk/tree/main/.github/.github/skills)  `/.github/skills` folder on your **local machine**, refresh the skills with `/skill reload`.
+ Run with `/setup-ssh` on your **local machine**.

<a id="install-sdk"></a>
### Skill: Install ctrlX Automation SDK on App Build Environment (QEMU VM)
Name: `/install-sdk`

Use the skill to setup the SDK on the App Build Environment (QEMU VM).

**Prerequisites:** 
+ [VSCode ](https://code.visualstudio.com/) installed.
+ App Build Environment (QEMU VM) is created and running.
+ SSH access to the App Build Environment (QEMU VM) configured. (use skill `/setup-ssh`).

**Usage**
+ Login into App Build Environment (QEMU VM), using [VSCode Remote-SSH Explorer](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-ssh).

+ Download the skill from [Skills](https://github.com/boschrexroth/ctrlx-automation-sdk/tree/main/.github/.github/skills) to `/.github/skills` folder on your **App Build Environment (QEMU VM)**, refresh the skills with `/skill reload`.

+ Run with `/install-sdk` on the **App Build Environment (QEMU VM)**.