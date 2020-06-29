import matplotlib.pyplot as plt
import numpy as np

def plot_result(opening_transform, fractures, vugs, t, cm='gnuplot', 
                  bins_shrink=1, t_color='red',
                  image_title="Input Image", hist_title="Histogram",
                  thresh_title1="Binary image", filter_background=-1, log=True):  
    fig_scale = 11
    fig = plt.figure(constrained_layout=True)
    aspect_ratio = (opening_transform.shape[0]/opening_transform.shape[1])
    fig.set_size_inches(fig_scale,fig_scale*aspect_ratio)
    widths = [1,1]
    heights = [1,1]
    spec = fig.add_gridspec(ncols=2, nrows=2, width_ratios=widths, height_ratios=heights)
    
    cm_binary = plt.cm.get_cmap('binary')
    colormap = plt.cm.get_cmap(cm)
    x = np.linspace(0, opening_transform.shape[1], num=5)
    xticks = np.linspace(0, 360, num=5).astype(int)
    
    # [1] Opening transform plot
    ax = fig.add_subplot(spec[0,0])
    plt.xticks(x, xticks)
    im = ax.imshow(opening_transform, cmap=colormap, interpolation='nearest', aspect= 'auto',
                   vmin=0)
    ax.set_title("Opening transform", fontdict={'size': 18})
    
    # [2] Histogram plot
    foreground = opening_transform[opening_transform > filter_background]
    bins = int(foreground.max()/bins_shrink);
    Y,X = np.histogram(foreground, bins)
    x_span = X.max()-X.min()
    C = [colormap(((x-X.min())/x_span)) for x in X]

    ax = fig.add_subplot(spec[0,1])
    plt.bar(X[:-1],Y,color=C,width=X[1]-X[0])
    #add threshold marker
    ax.text(t+8, Y.max()*0.95, "L = {}".format(int(t)), color=t_color, fontsize=16, weight='bold')
    plt.axvline(x=t, lw=1, c=t_color)
    ax.set_title(hist_title, fontdict={'size': 18})
    if log:
        ax.set_yscale('log')
    
    #[3] Extracted fractures plot
    ax = fig.add_subplot(spec[1,0])
    plt.xticks(x, xticks)
    im = ax.imshow(fractures, cmap=cm_binary, interpolation='nearest', aspect= 'auto', vmin=0)
    ax.set_title("Fractures (val > L)", fontdict={'size': 18})
    
    #[4] Extracted vugs plot
    ax = fig.add_subplot(spec[1,1])
    plt.xticks(x, xticks)
    im = ax.imshow(vugs, cmap=cm_binary, interpolation='nearest', aspect= 'auto', vmin=0)
    ax.set_title("Vugs (val < L)", fontdict={'size': 18})
    
    plt.show()