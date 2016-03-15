from setuptools import setup

version = '0.4'

long_description = '\n\n'.join([
    open('README.rst').read(),
    open('CREDITS.rst').read(),
    open('CHANGES.rst').read(),
    ])

install_requires = [
    'setuptools',
    'pyserial',
    ],

tests_require = [
    'nose',
    'coverage',
    ]

setup(name='reinout-arduino',
      version=version,
      description="Reinout's model railway arduino management project",
      long_description=long_description,
      # Get strings from http://www.python.org/pypi?%3Aaction=list_classifiers
      classifiers=[],
      keywords=[],
      author='Reinout van Rees',
      author_email='reinout@vanrees.org',
      url='https://github.com/reinout/reinout-arduino',
      license='MIT',
      packages=['reinout_arduino'],
      zip_safe=False,
      install_requires=install_requires,
      tests_require=tests_require,
      extras_require={'test': tests_require},
      entry_points={
          'console_scripts': [
              'update-libraries = reinout_arduino.libraries:main',
              'print-serial = reinout_arduino.serial:main',
          ]},
      )
