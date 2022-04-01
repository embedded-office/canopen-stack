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

```bash
# deploy a new website version
$ mike deploy --config-file ./website/mkdocs.yml --push --update-aliases vx.y.z latest

# manage the alias 'latest'
$ mike set-default --config-file ./website/mkdocs.yml --push latest

# list all website versions
$ mike delete --config-file ./website/mkdocs.yml [version-or-alias]

# delete a website version
$ mike delete --config-file ./website/mkdocs.yml [version-or-alias]
```

### Automatic deployment

The GitHub Action `website.yml` is defined to deploy (or overwrite) a website version. This action is triggered when a new version tag is created (naming pattern `v*.*.*`). A website update is triggered, when a file is changed within the directory `/website/...`.
