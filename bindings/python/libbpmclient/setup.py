import libbpmclient
from setuptools import setup, find_packages

readme = open('README.txt').read()

setup(
    name='libbpmclient',
    version=libbpmclient.build_revision,
    author='LNLS',
    author_email='lucas.russo@lnls.br',
    packages=find_packages(),
    scripts=[],
    url='https://github.com/lnls-dig/bpm-sw',
    license='COPYING',
    description='Library for interfacing with the LNLS BPM server',
    long_description=readme,
    install_requires=[]
)
