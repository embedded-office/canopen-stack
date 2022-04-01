# Website

This directory contains the whole content for the **CANopen Stack Project** website. The site is a static generated website out of a collection of markdown files. The used generator is [Material for MkDocs](https://squidfunk.github.io/mkdocs-material/). The resulting website is hosted on *GitHub Pages*.

## Contribution

In case you want to enhance the website content, you need to install MkDocs:

### Prepare Environment

```bash
# ensure python3 is installed (on my machine at time of writing: 3.9.x)
$ py --version

# install static website generator: MkDocs
$ py -m pip install -r ./website/requirements.txt
```

### Local server

During writing new content, a hot-reloading website is helpful. Start your server on your local machine:

```bash
# start development server in repository root (unix, powershell)
$ mkdocs serve --config-file ./website/mkdocs.yml
```

### Deploy website manually

When deployment of the website via commandline is required, you can do it by typing

TODO: check and describe the commands for website version management

```bash
# deploy a new website version
$ TODO: mike ...

# manage the alias 'latest'
$ TODO: mike ...

# list all website versions
$ TODO: mike ...

# delete a website version
$ TODO: mike ...
```

### Automatic deployment

The GitHub Action `website.yml` is defined to deploy (or overwrite) a website version. This action is triggered when a new version tag is created (naming pattern `v*.*.*`) or a file is changed within the directory `/website/...`.
