semver = require 'semver'
fs = require 'fs'
path = require 'path'

HtmlWebpackPlugin = require 'html-webpack-plugin'

packageInfo = require './package.json'

currentNodeVersion = process.version
expectedNodeVersion = fs.readFileSync '.nvmrc','utf-8' # must specify encoding to read as text.
if expectedNodeVersion and not semver.satisfies currentNodeVersion,expectedNodeVersion
	colors = require 'colors'
	console.error "Invalid node version. Expected: #{expectedNodeVersion} found: #{currentNodeVersion}".red
	process.exit 1

adjustConfig = (webpackConfig, options={})->
	config = Object.assign {}, webpackConfig
	if options.devMode
		config.mode = 'development'
		config.devtool = 'inline-source-map'
	else
		config.mode = 'production'
		delete config.devtool

	config

webpackConfig =
	renderer:
		context: path.join __dirname,'src/renderer'
		entry: './scripts/main.coffee'
		output:
			filename: 'renderer.js'
			path: path.join __dirname, '<%= paths.build %>', 'renderer'
		target: 'electron-renderer'
		module:
			rules: [
				test: /\.s?css$/
				use: [
					'style-loader'
					'css-loader'
					'sass-loader'
				]
			,
				test: /\.coffee$/
				use: [ 'coffee-loader' ]
			,
				test: /\.hbs$/
				use: [ 'handlebars-loader' ]
			,
				test: /\.(woff(2)?|ttf|eot|svg)(\?v=\d+\.\d+\.\d+)?$/,
				use: [
						loader: 'file-loader'
						options:
							name: '[name].[ext]'
							outputPath: 'fonts/'
				]
			,
				test: /\.(png|svg|jpg|gif)$/
				use: [
						loader: 'file-loader'
						options:
							outputPath: 'images/'
				]
			]
		plugins: [
			new HtmlWebpackPlugin
				template: 'index.html'
		]
		resolve:
			extensions: ['.js','.coffee','.hbs']
			modules: ['scripts','node_modules']
			alias:
				packageInfo$: path.resolve __dirname, 'package.json'

module.exports = (grunt)->
	# Load grunt tasks automatically
	require('load-grunt-tasks')(grunt)

	# Define the configuration for all the tasks
	grunt.initConfig

		packageInfo: packageInfo
		electronVersion: packageInfo.devDependencies['electron']

		paths:
			src: 'src'
			dist: 'dist'
			build: 'build'

		clean:
			dist: '<%= paths.dist %>'
			build: '<%= paths.build %>'

		copy:
			main:
				files: [
					expand: true
					cwd: 'src/main'
					src: ['**']
					dest: '<%= paths.build %>'
				,
					expand: true
					dest: '<%= paths.build %>'
					src: ['package.json']
				]

			nodeModules:
				files: [
					expand: true
					cwd: 'node_modules'
					dest: '<%= paths.build %>/node_modules'
					src: ['**']
				]

		env:
			dev:
				NODE_ENV: 'development'
			prod:
				NODE_ENV: 'production'

		webpack:
			rendererDev: adjustConfig webpackConfig.renderer,
				devMode: true

			# 	devMode: false
			rendererProd: adjustConfig webpackConfig.renderer,
				devMode: false

		exec:
			electron:
				command: "npx electron <%= paths.build %>"

		electron:
			options:
				'appBundleId': packageInfo.applicationId
				name: packageInfo.displayName
				dir: path.resolve __dirname, '<%= paths.build %>'
				out: path.resolve __dirname, '<%= paths.dist %>'
				electronVersion: grunt.config.get 'electronVersion' # use same version as during dev.
				appVersion: packageInfo.version
				# asar: false
				asar: true
				overwrite: true
				icon: 'images/icon'
				prune: true
			osx:
				options:
					platform: 'darwin'
					arch: 'x64'

		watch:
			mainDev:
				files: [path.resolve __dirname, '<%= paths.src %>', 'main', '**/*']
				tasks: ['copy:main']
			rendererDev:
				files: [path.resolve __dirname, '<%= paths.src %>', 'renderer', '**/*']
				tasks: ['webpack:rendererDev']

		concurrent:
			serve:
				tasks: [
					'watch'
					'exec:electron'
				]
				options:
					logConcurrentOutput: true

	grunt.registerTask 'serve', [
		'env:dev'
		'clean'

		'webpack:rendererDev'
		'copy:main'

		'concurrent:serve'
	]

	grunt.registerTask 'dist', [
		'env:prod'
		'clean'

		'webpack:rendererProd'
		'copy'

		'electron'
	]
