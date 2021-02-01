HtmlWebpackPlugin = require 'html-webpack-plugin'
path = require 'path'

module.exports =
	mode: 'development'
	devServer:
		contentBase: path.join __dirname,'dist'
	devtool: 'inline-source-map'
	context: path.join __dirname,'src'
	entry: './scripts/main.coffee'
	output:
		filename: 'main.js'
		path: path.resolve __dirname, 'dist'
	module:
		rules: [
			test: /\.coffee$/,
			loader: 'coffee-loader'
		,
			test: /\.s?css$/
			use: [
				'style-loader',
				'css-loader',
				'sass-loader'
			]
		,
			test: /\.hbs$/
			loader: 'handlebars-loader'
			options:
				helperDirs: [path.resolve 'src/scripts/templates/helpers']
		]
	plugins: [
		new HtmlWebpackPlugin
			template: './index.html'
	]
	resolve:
		extensions: ['.js','.coffee','.hbs']
		modules: ['node_modules','src/scripts']
