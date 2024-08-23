# VisionGuard Application Benchmark Results

## M3 Pro (18GB memory)

| Quant | Device | Limit | FPS | Latency (ms) | CPU (%) | RAM (MB) |
|-------|--------|-------|-----|--------------|---------|----------|
| INT-8 FP-16 | CPU | 10 - FPS | 8.8 | 26.6 | 172.0 | 345.9 |
| FP-16 | CPU | 10 - FPS | 7.7 | 20.2 | 113.0 | 432.7 |
| FP-32 | CPU | 10 - FPS | 7.9 | 26.1 | 115.3 | 280.0 |
| INT-8 FP-16 | CPU | 25 - FPS | 19.0 | 16.4 | 260.0 | 347.0 |
| FP-16 | CPU | 25 - FPS | 19.0 | 13.7 | 225.0 | 430.0 |
| FP-32 | CPU | 25 - FPS | 19.5 | 13.6 | 202.2 | 281.6 |
| INT-8 FP-16 | CPU | Max FPS | 30.4 | 14.4 | 370.2 | 343.0 |
| FP-16 | CPU | Max FPS | 30.2 | 10.6 | 291.0 | 434.6 |
| FP-32 | CPU | Max FPS | 30.1 | 10.6 | 254.0 | 280.5 |

## Intel AI-PC

### NPU Performance

| Quant | Device | Limit | FPS | Latency (ms) | CPU (%) | NPU (%) | NPU Memory (MB) | RAM (MB) |
|-------|--------|-------|-----|--------------|---------|---------|-----------------|----------|
| FP-16 | NPU | 10 - FPS | 7.1 | 13.4 | 1.1 | 11.0 | 200 | 246.0 |
| FP-16 | NPU | 25 - FPS | 21.0 | 12.3 | 2.1 | 23.0 | 200 | 251.0 |
| FP-16 | NPU | Max FPS | 21.6 | 12.0 | 2.8 | 26.0 | 200 | 247.0 |
| FP-32 | NPU | 10 - FPS | 7.1 | 13.5 | 1.8 | 10.0 | 200 | 257.9 |
| FP-32 | NPU | 25 - FPS | 21.3 | 13.3 | 2.7 | 24.0 | 200 | 257.0 |
| FP-32 | NPU | Max FPS | 21.4 | 13.6 | 3.1 | 25.0 | 200 | 257.8 |
| INT-8 FP-16 | NPU | 10 - FPS | 7.9 | 7.0 | 1.7 | 6.0 | 200 | 223.7 |
| INT-8 FP-16 | NPU | 25 - FPS | 21.4 | 7.1 | 3.0 | 10.0 | 200 | 229.7 |
| INT-8 FP-16 | NPU | Max FPS | 21.3 | 7.0 | 3.4 | 11.0 | 200 | 230.6 |

### CPU Performance

| Quant | Device | Limit | FPS | Latency (ms) | CPU (%) | RAM (MB) |
|-------|--------|-------|-----|--------------|---------|----------|
| FP-16 | CPU | 10 - FPS | 7.2 | 10.5 | 4.1 | 302.6 |
| FP-16 | CPU | 25 - FPS | 20.3 | 11.2 | 6.5 | 301.7 |
| FP-16 | CPU | Max FPS | 21.2 | 12.3 | 7.0 | 309.8 |
| FP-32 | CPU | 10 - FPS | 7.9 | 10.2 | 4.5 | 278.3 |
| FP-32 | CPU | 25 - FPS | 20.1 | 13.1 | 7.9 | 275.6 |
| FP-32 | CPU | Max FPS | 21.2 | 12.6 | 8.3 | 278.2 |
| INT-8 FP-16 | CPU | 10 - FPS | 7.1 | 6.3 | 2.3 | 248.7 |
| INT-8 FP-16 | CPU | 25 - FPS | 21.2 | 5.6 | 4.4 | 248.7 |
| INT-8 FP-16 | CPU | Max FPS | 21.4 | 6.1 | 4.8 | 245.7 |

### IGPU Performance

| Quant | Device | Limit | FPS | Latency (ms) | CPU (%) | GPU (%) | GPU Shared Memory (GB) | RAM (MB) |
|-------|--------|-------|-----|--------------|---------|---------|------------------------|----------|
| FP-16 | IGPU | 10 - FPS | 7.1 | 4.6 | 1.6 | 19.0 | 2.1 | 354.2 |
| FP-16 | IGPU | 25 - FPS | 21.3 | 4.2 | 2.1 | 54.0 | 2.1 | 354.8 |
| FP-16 | IGPU | Max FPS | 21.5 | 4.3 | 3.0 | 56.0 | 2.1 | 354.3 |
| FP-32 | IGPU | 10 - FPS | 7.0 | 4.5 | 1.9 | 19.0 | 2.1 | 317.7 |
| FP-32 | IGPU | 25 - FPS | 20.7 | 4.2 | 4.0 | 60.0 | 2.1 | 311.3 |
| FP-32 | IGPU | Max FPS | 21.3 | 4.6 | 4.3 | 61.0 | 2.1 | 317.7 |
| INT-8 FP-16 | IGPU | 10 - FPS | 7.1 | 3.9 | 2.3 | 20.0 | 2.1 | 369.0 |
| INT-8 FP-16 | IGPU | 25 - FPS | 21.0 | 3.6 | 3.5 | 49.0 | 2.1 | 388.9 |
| INT-8 FP-16 | IGPU | Max FPS | 21.3 | 3.4 | 3.8 | 51.0 | 2.1 | 338.0 |

## Asus TUF A-15

### AUTO Performance

| Quant | Device | Limit | FPS | Latency (ms) | CPU (%) | Intel GPU (%) | Intel GPU Memory (MB) | RAM (MB) |
|-------|--------|-------|-----|--------------|---------|---------------|------------------------|----------|
| FP-16 | AUTO | 10 - FPS | 7.6 | 18.5 | 1.5 | 8 | 400 | 520.6 |
| FP-16 | AUTO | 25 - FPS | 17.7 | 17.0 | 2.7 | 16 | 400 | 521.0 |
| FP-16 | AUTO | Max FPS | 20.7 | 16.6 | 5.5 | 29 | 400 | 518.7 |
| FP-32 | AUTO | 10 - FPS | 7.3 | 19.7 | 1.2 | 11 | 400 | 497.7 |
| FP-32 | AUTO | 25 - FPS | 17.6 | 17.8 | 2.5 | 17 | 400 | 499.7 |
| FP-32 | AUTO | Max FPS | 20.9 | 16.5 | 2.6 | 20 | 400 | 499.2 |
| INT-8 FP-16 | AUTO | 10 - FPS | 7.9 | 9.5 | 0.9 | 6 | 300 | 315.0 |
| INT-8 FP-16 | AUTO | 25 - FPS | 17.3 | 9.0 | 3.0 | 9 | 300 | 318.0 |
| INT-8 FP-16 | AUTO | Max FPS | 20.4 | 8.3 | 4.9 | 12 | 300 | 320.7 |

### CPU Performance

| Quant | Device | Limit | FPS | Latency (ms) | CPU (%) | Intel GPU (%) | Intel GPU Memory (MB) | RAM (MB) |
|-------|--------|-------|-----|--------------|---------|---------------|------------------------|----------|
| FP-16 | CPU | 10 - FPS | 7.7 | 7.2 | 2.7 | 2 | 300 | 331.5 |
| FP-16 | CPU | 25 - FPS | 20.8 | 6.7 | 5.8 | 2 | 300 | 330.0 |
| FP-16 | CPU | Max FPS | 20.9 | 7.4 | 6.8 | 2 | 300 | 331.0 |
| FP-32 | CPU | 10 - FPS | 7.4 | 7.6 | 3.3 | 2 | 300 | 295.9 |
| FP-32 | CPU | 25 - FPS | 17.6 | 7.5 | 5.4 | 2 | 300 | 296.5 |
| FP-32 | CPU | Max FPS | 20.4 | 7.6 | 6.1 | 2 | 300 | 295.0 |
| INT-8 FP-16 | CPU | 10 - FPS | 7.4 | 5.5 | 1.6 | 2 | 300 | 279.3 |
| INT-8 FP-16 | CPU | 25 - FPS | 19.3 | 4.9 | 4.8 | 2 | 300 | 276.7 |
| INT-8 FP-16 | CPU | Max FPS | 20.9 | 5.4 | 6.7 | 2 | 300 | 279.7 |

### IGPU Performance

| Quant | Device | Limit | FPS | Latency (ms) | CPU (%) | Intel GPU (%) | Intel GPU Memory (MB) | RAM (MB) |
|-------|--------|-------|-----|--------------|---------|---------------|------------------------|----------|
| FP-16 | IGPU | 10 - FPS | 7.5 | 12.8 | 1.2 | 10 | 400 | 416.3 |
| FP-16 | IGPU | 25 - FPS | 19.6 | 10.7 | 1.5 | 16 | 400 | 416.1 |
| FP-16 | IGPU | Max FPS | 21.0 | 11.4 | 1.4 | 20 | 400 | 416.6 |
| FP-32 | IGPU | 10 - FPS | 7.0 | 12.5 | 1.3 | 9 | 400 | 430.1 |
| FP-32 | IGPU | 25 - FPS | 18.5 | 12.0 | 1.3 | 18 | 400 | 430.4 |
| FP-32 | IGPU | Max FPS | 20.9 | 12.2 | 1.4 | 26 | 400 | 433.1 |
| INT-8 FP-16 | IGPU | 10 - FPS | 7.7 | 13.4 | 1.0 | 8 | 400 | 519.9 |
| INT-8 FP-16 | IGPU | 25 - FPS | 20.6 | 13.0 | 1.4 | 16 | 400 | 515.2 |
| INT-8 FP-16 | IGPU | Max FPS | 21.1 | 12.8 | 1.9 | 18 | 400 | 517.7 |

## Final Inference

1. Quantization Impact:
   - INT-8 FP-16 quantization generally provides the best balance between performance and resource utilization across all devices.
   - FP-16 and FP-32 quantizations show similar performance in most cases, with FP-16 having a slight edge in terms of memory usage.

2. Device Performance:
   - GPU consistently outperforms CPU and NPU in terms of FPS and latency.
   - NPU shows efficient resource utilization but has limitations in maximum achievable FPS.
   - CPU performance varies significantly across different hardware configurations.

3. FPS Limits:
   - Higher FPS limits generally lead to increased resource utilization (CPU, GPU, NPU, and RAM).
   - The relationship between FPS and latency is not always linear, with some configurations showing improved latency at higher FPS.

4. Hardware Differences:
   - The M3 Pro shows high CPU utilization but achieves the highest FPS.
   - The Intel AI-PC demonstrates balanced performance across CPU, NPU, and GPU.
   - The Asus TUF A-15 shows good performance with its integrated GPU (IGPU).

5. RAM Usage:
   - RAM usage varies across different quantizations and devices, with FP-16 generally using more RAM than INT-8 FP-16 and FP-32.

## Recommendations

1. Quantization: Use INT-8 FP-16 quantization when possible, as it provides the best balance between performance and resource utilization.

2. Device Selection:
   - For high-performance requirements, prioritize GPU usage.
   - For energy efficiency and balanced performance, consider NPU on supported hardware.
   - Use CPU as a fallback option or for systems without dedicated AI accelerators.

3. FPS Limits: Choose FPS limits based on the specific use case requirements. Higher FPS may not always be necessary and can lead to increased resource consumption.

4. Hardware Considerations:
   - For maximum performance, the M3 Pro is the best choice, especially if high CPU utilization is acceptable.
   - For a balance of performance and efficiency, the Intel AI-PC with NPU is recommended.
   - For systems with integrated graphics, like the Asus TUF A-15, leveraging the IGPU can provide
