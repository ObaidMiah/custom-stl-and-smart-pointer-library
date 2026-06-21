#!/usr/bin/env python3
"""
Generate interactive Plotly HTML charts for vector performance visualization.
"""

import json
import plotly.graph_objects as go
import os

def load_benchmark_data():
    """Load benchmark data from JSON file."""
    try:
        # Get the project root directory (two levels up from scripts)
        script_dir = os.path.dirname(os.path.abspath(__file__))
        project_root = os.path.dirname(os.path.dirname(script_dir))
        data_file = os.path.join(project_root, 'data', 'benchmark_data.json')
        

        
        with open(data_file, 'r') as f:
            return json.load(f)
    except FileNotFoundError:
        print("Error: benchmark_data.json not found!")
        print("Please run the benchmark program first: cd build && ./benchmark_vector")
        return None
    except json.JSONDecodeError:
        print("Error: Invalid JSON in benchmark_data.json")
        return None

def create_performance_comparison_chart(data):
    """Create the main performance comparison chart."""
    fig = go.Figure()
    
    # Handle zero values by replacing them with a small positive value for log scale
    min_time = 0.0001  # 0.1 microseconds
    custom_times = [max(time, min_time) for time in data['custom_vector_times']]
    std_times = [max(time, min_time) for time in data['std_vector_times']]
    const_times = [max(time, min_time) for time in data['const_vector_times']]

    # Custom Vector line with enhanced styling
    fig.add_trace(go.Scatter(
        x=data['test_sizes'],
        y=custom_times,
        mode='lines+markers',
        name='Exponential Vector',
        line=dict(color='#2E86AB', width=4, shape='spline', smoothing=0.3),
        marker=dict(
            size=8, 
            color='#2E86AB',
            line=dict(width=2, color='white'),
            symbol='circle'
        ),
        hovertemplate='<b>Custom Vector</b><br>' +
                     'Size: %{x:,}<br>' +
                     'Time: %{y:.3f} ms<br>' +
                     '<extra></extra>',
        showlegend=True
    ))
    
    # Const Vector line with enhanced styling
    fig.add_trace(go.Scatter(
        x=data['test_sizes'],
        y=const_times,
        mode='lines+markers',
        name='Constant Vector',
        line=dict(color='#F18F01', width=4, shape='spline', smoothing=0.3),
        marker=dict(
            size=8, 
            color='#F18F01',
            line=dict(width=2, color='white'),
            symbol='square'
        ),
        hovertemplate='<b>Const Vector</b><br>' +
                     'Size: %{x:,}<br>' +
                     'Time: %{y:.3f} ms<br>' +
                     '<extra></extra>',
        showlegend=True
    ))

    # std::vector line with enhanced styling
    fig.add_trace(go.Scatter(
        x=data['test_sizes'],
        y=std_times,
        mode='lines+markers',
        name='std::vector',
        line=dict(color='#A23B72', width=4, shape='spline', smoothing=0.3),
        marker=dict(
            size=8, 
            color='#A23B72',
            line=dict(width=2, color='white'),
            symbol='diamond'
        ),
        hovertemplate='<b>std::vector</b><br>' +
                     'Size: %{x:,}<br>' +
                     'Time: %{y:.3f} ms<br>' +
                     '<extra></extra>',
        showlegend=True
    ))
    
    # Update layout with enhanced styling
    fig.update_layout(
        title={
            'text': 'Vector Performance Comparison<br><sub>Exponential, Constant, and std::vector Implementations</sub>',
            'x': 0.5,
            'xanchor': 'center',
            'font': {'size': 28, 'color': '#1a1a1a', 'family': 'Arial, sans-serif'}
        },
        xaxis_title='Input Size (Number of Elements)',
        yaxis_title='Execution Time (milliseconds)',
        xaxis_type='log',
        yaxis_type='linear',
        hovermode='x unified',
        template='plotly_white',
        font=dict(family='Arial, sans-serif', size=12, color='#333333'),
        plot_bgcolor='#fafafa',
        paper_bgcolor='white',
        margin=dict(l=100, r=80, t=120, b=80),
        legend=dict(
            x=0.02,
            y=0.98,
            bgcolor='rgba(255,255,255,0.95)',
            bordercolor='#cccccc',
            borderwidth=1,
            font=dict(size=14),
            itemsizing='constant'
        ),
        width=1000,
        height=600
    )
    
    # Update axes with enhanced styling
    fig.update_xaxes(
        title_font=dict(size=14, color='#333333'),
        tickfont=dict(size=11, color='#666666'),
        gridcolor='rgba(0,0,0,0.08)',
        showline=True,
        linewidth=2,
        linecolor='#cccccc',
        tickformat=',',
        zeroline=False,
        showgrid=True,
        gridwidth=1,
        type='log'
    )
    
    fig.update_yaxes(
        title_font=dict(size=14, color='#333333'),
        tickfont=dict(size=11, color='#666666'),
        gridcolor='rgba(0,0,0,0.08)',
        showline=True,
        linewidth=2,
        linecolor='#cccccc',
        zeroline=True,
        zerolinecolor='#cccccc',
        zerolinewidth=1,
        showgrid=True,
        gridwidth=1,
        type='linear',
        range=[0, 1.0],  # Start at 0, go to 1.0ms
        dtick=0.1,       # Increment by 0.1ms
        tickmode='linear',
        showticklabels=True,
        nticks=None      # Show all ticks
    )
    
    return fig



def main():
    """Main function to generate performance chart."""
    print("Generating performance chart...")
    
    # Load data
    data = load_benchmark_data()
    if not data:
        return
    
    print(f"Loaded data for {len(data['test_sizes'])} test sizes")
    
    # Create performance chart
    print("Creating performance comparison chart...")
    perf_chart = create_performance_comparison_chart(data)
    output_path = '../charts/performance_chart.html'
    perf_chart.write_html(output_path, include_plotlyjs='cdn')
    
    print(f"Performance chart generated successfully: {output_path}")

if __name__ == "__main__":
    main()
