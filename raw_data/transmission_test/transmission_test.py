import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import scipy.stats as stats

# 1. LOAD AND CLEAN DATA
df = pd.read_csv("transmission_test.csv")
df.columns = [c.strip().replace('"', '') for c in df.columns]

# Rename columns
df.rename(columns={
    'Force input (N)': 'F_in',
    'Force output (N)': 'F_out'
}, inplace=True)

# 2. TRANSFORM ALL DATA
df['mu_static'] = np.log(df['F_in'] / df['F_out']) / np.pi

# 3. CREATE ANALYSIS SUBSET (9N - 18N)
df_analysis = df[~df['F_in'].isin([3, 6])].copy()
df_omitted = df[df['F_in'].isin([3, 6])].copy()

# 4. CALCULATE STATISTICS (Mean and 95% CI)
# We treat the 9-18N data as a single sample to find the aggregate mean and CI
mu_values = df_analysis['mu_static']
mean_mu = mu_values.mean()
sem = stats.sem(mu_values)  # Standard Error of the Mean

# Calculate 95% Confidence Interval using t-distribution
confidence = 0.95
ci = stats.t.interval(confidence, len(mu_values)-1, loc=mean_mu, scale=sem)

print(f"Mean (9N-18N): {mean_mu:.4f}")
print(f"95% Confidence Interval: [{ci[0]:.4f}, {ci[1]:.4f}]")

# 5. PLOTTING WITH JITTER
plt.figure(figsize=(10, 6))

jitter_val = 0.15 
np.random.seed(42)

# Plot Omitted Data (3N, 6N)
x_omitted_jittered = df_omitted['F_in'] + np.random.normal(0, jitter_val, size=len(df_omitted))
plt.scatter(x_omitted_jittered, df_omitted['mu_static'], 
            color='gray', label='Omitted Data (3N, 6N)', alpha=0.3, edgecolors='k', marker='x')

# Plot Analyzed Data (9N-18N)
x_analysis_jittered = df_analysis['F_in'] + np.random.normal(0, jitter_val, size=len(df_analysis))
plt.scatter(x_analysis_jittered, df_analysis['mu_static'], 
            color='#2ca02c', label='Analyzed Data (9N-18N)', alpha=0.6, edgecolors='k')

# Plot Mean and Confidence Interval Band
# axhspan plots a horizontal shaded region across the entire x-axis
plt.axhline(mean_mu, color='#ff7f0e', linestyle='--', linewidth=2, label=f'Mean: {mean_mu:.3f}')
plt.axhspan(ci[0], ci[1], color='#ff7f0e', alpha=0.2, label='95% CI')

# Formatting
plt.xticks(np.arange(3, 21, 3))
plt.xlim(2, 19)
plt.title('Friction Coefficient Analysis (Mean & 95% CI)', fontsize=14)
plt.xlabel('Input Force $F_{in}$ (N)', fontsize=12)
plt.ylabel(r'Static Friction $\mu$ = $\ln(\frac{F_{in}}{F_{out}}) / \pi$', fontsize=12)
plt.legend(loc='upper right')
plt.grid(True, linestyle='--', alpha=0.5)

plt.tight_layout()
plt.savefig('confidence_interval_plot.png', dpi=300)
plt.show()