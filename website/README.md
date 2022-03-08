# Website

This directory contains the whole content for the **CANopen Stack Project** website. The site is a static generated website out of a collection of markdown files. The used generator is [Material for MkDocs](https://squidfunk.github.io/mkdocs-material/). The resulting website is hosted on *GitHub Pages*.

## Contribution

In case you want to enhance the website content, you need to fetch the Docker image: `squidfunk/mkdocs-material`

```bash
# get docker image
$ docker pull squidfunk/mkdocs-material:8.2.5

# start development server in repository root (unix, powershell)
$ docker run --rm -it -v ${PWD}\docs:/docs -p 8000:8000 squidfunk/mkdocs-material:8.2.5
```
