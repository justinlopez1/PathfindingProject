# other package imports
import matplotlib.pyplot as plt  # keep this import for CI to work
from zanj import ZANJ  # saving/loading data
from muutils.mlutils import pprint_summary  # pretty printing as json

# maze_dataset imports
from maze_dataset import LatticeMaze, SolvedMaze, MazeDataset, MazeDatasetConfig
from maze_dataset.generation import LatticeMazeGenerators, GENERATORS_MAP
from maze_dataset.generation.default_generators import DEFAULT_GENERATORS
from maze_dataset.dataset.configs import MAZE_DATASET_CONFIGS
from maze_dataset.plotting import plot_dataset_mazes, print_dataset_mazes
from maze_dataset.plotting import MazePlot
from maze_dataset.tokenization import MazeTokenizer, TokenizationMode
from maze_dataset.plotting.print_tokens import display_color_maze_tokens_AOTP, color_maze_tokens_AOTP
from maze_dataset.dataset.rasterized import process_maze_rasterized_input_target

# check the configs
print(MAZE_DATASET_CONFIGS.keys())
# for saving/loading things
LOCAL_DATA_PATH: str = "../data/maze_dataset/"
zanj: ZANJ = ZANJ(external_list_threshold=256)

cfg: MazeDatasetConfig = MazeDatasetConfig(
    name="test",  # name is only for you to keep track of things
    grid_n=24,  # number of rows/columns in the lattice
    n_mazes=100,  # number of mazes to generate
    maze_ctor=LatticeMazeGenerators.gen_dfs,  # algorithm to generate the maze
    # there are a few more arguments here, to be discussed later
)

# each config will use this function to get the name of the dataset
# it contains some basic info about the algorithm, size, and number of mazes
# at the end after "h" is a stable hash of the config to avoid collisios
print(cfg.to_fname())

# to create a dataset, just call MazeDataset.from_config
dataset: MazeDataset = MazeDataset.from_config(
    # your config
    cfg,
    # and all this below is completely optional
    do_download=False,
    load_local=False,
    do_generate=True,
    save_local=True,
    local_base_path=LOCAL_DATA_PATH,
    verbose=True,
    zanj=zanj,
    gen_parallel=False,  # parallel generation has overhead, not worth it unless you're doing a lot of mazes
)

plot_dataset_mazes(dataset, count=None)  # for large datasets, set the count to some int to just plot the first few

dataset_filtered: MazeDataset = dataset.filter_by.path_length(min_length=3)

print(f"{len(dataset) = }")
print(f"{len(dataset_filtered) = }")

plot_dataset_mazes(dataset_filtered)

pprint_summary(dataset_filtered.cfg.serialize()["applied_filters"])
print(f"{MazeDataset._FILTER_NAMESPACE = }")

# filters can also be specified at generation time -- but it will still generate the whole dataset and then filter it

dataset_filtered_from_scratch: MazeDataset = MazeDataset.from_config(
    dataset_filtered.cfg,
    do_download=False,
    load_local=False,
    do_generate=True,
    save_local=False,
    local_base_path=LOCAL_DATA_PATH,
    verbose=True,
    zanj=zanj,
    gen_parallel=False,
)

plot_dataset_mazes(dataset_filtered_from_scratch)
dataset_filtered_nodupe = dataset_filtered_from_scratch.filter_by.remove_duplicates()
plot_dataset_mazes(dataset_filtered_nodupe)

dataset_with_meta = dataset.filter_by.collect_generation_meta()
metadata = dataset_with_meta.serialize()['generation_metadata_collected']
metadata["visited_cells"] = "..." # this is a huge list and unweildy to print
pprint_summary(metadata)

maze: SolvedMaze = dataset_filtered_nodupe[1]

# as pixels (what you've already seen)
plt.imshow(maze.as_pixels())

# as ascii (useful for debugging)
print("ASCII:\n")
print(maze.as_ascii())

#tempFile = open(r"7x7.txt", "a")
#tempFile = open(r"9x9.txt", "a")
#tempFile = open(r"11x11.txt", "a")
#tempFile = open(r"13x13.txt", "a")
#tempFile = open(r"15x15.txt", "a")
#tempFile = open(r"17x17.txt", "a")
#tempFile = open(r"19x19.txt", "a")
#tempFile = open(r"21x21.txt", "a")
#tempFile = open(r"23x23.txt", "a")
#tempFile = open(r"25x25.txt", "a")
tempFile = open(r"49x49.txt", "a")




for x in dataset:
    maze: SolvedMaze = x
    tempFile.write(maze.as_ascii() + "\n")

# as e2h style input/target
input, target = process_maze_rasterized_input_target(maze)
fig, ax = plt.subplots(1, 2)
ax[0].imshow(input)
ax[1].imshow(target)
# remove ticks
for a in ax:
	a.set_xticks([])
	a.set_yticks([])
plt.show()


# as a MazePlot
MazePlot(maze).plot()
"""
# as tokens

# first, initialize a tokenizer -- more about this in the `notebooks/demo_tokenization.ipynb` notebook
tokenizer: MazeTokenizer = MazeTokenizer(tokenization_mode=TokenizationMode.AOTP_UT_rasterized, max_grid_size=100)
maze_tok = maze.as_tokens(maze_tokenizer=tokenizer)

# you can view the tokens directly
print("\nRaw tokens:\n")
print(" ".join(maze_tok))

# or color and print them in various formats
print("\nColored tokens, raw html:\n")
print(color_maze_tokens_AOTP(maze_tok, fmt="html"))
print("\nColored tokens, raw latex:\n")
print(color_maze_tokens_AOTP(maze_tok, fmt="latex"))
print("\nColored tokens, terminal:\n")
print(color_maze_tokens_AOTP(maze_tok, fmt="terminal"))

display_color_maze_tokens_AOTP(maze_tok)

"""