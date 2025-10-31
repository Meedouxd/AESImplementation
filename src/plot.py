import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

csv_folder = Path(__file__).parent / 'csv_files'
csv_files = list(csv_folder.glob('*.csv'))
output_folder = Path(__file__).parent / 'graphs'
output_folder.mkdir(exist_ok=True)

for csv_file in csv_files:
    df = pd.read_csv(csv_file)


    plt.figure(figsize=(10, 6))
    plt.plot(df.iloc[:, 0], color='blue', alpha=0.7)
    plt.title(f'Graph of {csv_file.stem}')
    plt.xlabel('Index')
    plt.ylabel('Value')
    plt.grid(True)

    output_path = output_folder / f'{csv_file.stem}.png'
    plt.savefig(output_path)
    plt.close()

    print(f"Saved {output_path}")
