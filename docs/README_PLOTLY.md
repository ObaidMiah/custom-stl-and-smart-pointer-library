# Plotly Chart Generation for Vector Performance

This directory contains Python scripts to generate beautiful, interactive HTML charts using Plotly for your vector performance visualization.

## 🚀 Quick Start

### 1. Generate Charts

```bash
cd docs/visualization
python3 generate_plotly_charts.py
```

This will create:

- `performance_chart.html` - Performance comparison chart
- `performance_ratio.html` - Performance ratio analysis
- `performance_dashboard.html` - Combined dashboard
- `plotly_showcase.html` - Showcase page with all charts

### 2. View Results

Open `plotly_showcase.html` in your browser to see all charts.

## 📊 What You Get

### Performance Comparison Chart

- **Interactive line chart** showing Custom Vector vs std::vector performance
- **Logarithmic scaling** for better data visualization
- **Hover tooltips** with detailed information
- **Zoom and pan** capabilities
- **Export options** (PNG, SVG, HTML)

### Performance Ratio Chart

- **Ratio analysis** (Custom Vector / std::vector)
- **Reference line** at ratio = 1.0 (equal performance)
- **Interpretation** (faster/slower/equal)
- **Interactive features** same as above

### Combined Dashboard

- **Two charts** in a single view
- **Professional layout** perfect for presentations
- **Responsive design** works on all devices

## 🌐 Website Deployment

### Option 1: Upload All Files

Upload these files to your web server:

- `plotly_showcase.html` (main page)
- `performance_chart.html`
- `performance_ratio.html`
- `performance_dashboard.html`

### Option 2: Embed Individual Charts

Use iframes to embed charts in other pages:

```html
<iframe src="performance_chart.html" width="100%" height="600px"></iframe>
```

### Option 3: Standalone Charts

Each chart is a complete HTML file that can be viewed independently.

## 🔧 Customization

### Modify Chart Styles

Edit `generate_plotly_charts.py` to change:

- Colors and themes
- Chart sizes and layouts
- Fonts and styling
- Chart types and features

### Add New Charts

Extend the script to create:

- Bar charts
- Scatter plots
- Heatmaps
- Statistical analysis charts

## 📱 Features

- **Responsive**: Works on desktop, tablet, and mobile
- **Interactive**: Zoom, pan, hover, and export
- **Professional**: Clean, modern design
- **Fast**: Lightweight HTML files
- **Compatible**: Works in all modern browsers

## 🎯 Benefits Over JavaScript Charts

1. **Python-powered**: Use your favorite data science libraries
2. **Better styling**: More professional appearance
3. **Export options**: Download as images or interactive HTML
4. **Easier customization**: Python is more flexible than JavaScript
5. **Better performance**: Optimized for large datasets

## 🔄 Updating Charts

When you have new benchmark data:

1. Run `./benchmark_vector` in the build directory
2. Copy `benchmark_data.json` to the `data/` directory
3. Run `python3 generate_plotly_charts.py`
4. Upload the new HTML files to your website

## 💡 Tips

- **Mobile optimization**: Charts automatically resize for mobile devices
- **SEO friendly**: HTML files are searchable
- **Fast loading**: Uses CDN for Plotly library
- **Professional**: Perfect for portfolios and presentations

---

**Your charts are now ready for website deployment! 🚀**
