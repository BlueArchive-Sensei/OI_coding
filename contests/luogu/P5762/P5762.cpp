#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

struct Satellite {
    int x, y, z, r;
    // 立方体范围：[x-r, x+r] × [y-r, y+r] × [z-r, z+r]
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<Satellite> satellites(n);
    
    // 收集所有坐标边界
    set<int> x_coords, y_coords, z_coords;
    
    for (int i = 0; i < n; i++) {
        cin >> satellites[i].x >> satellites[i].y >> satellites[i].z >> satellites[i].r;
        
        // 添加立方体的边界坐标
        x_coords.insert(satellites[i].x - satellites[i].r);
        x_coords.insert(satellites[i].x + satellites[i].r);
        y_coords.insert(satellites[i].y - satellites[i].r);
        y_coords.insert(satellites[i].y + satellites[i].r);
        z_coords.insert(satellites[i].z - satellites[i].r);
        z_coords.insert(satellites[i].z + satellites[i].r);
    }
    
    // 转换为有序数组进行离散化
    vector<int> x_arr(x_coords.begin(), x_coords.end());
    vector<int> y_arr(y_coords.begin(), y_coords.end());
    vector<int> z_arr(z_coords.begin(), z_coords.end());
    
    long long total_volume = 0;
    
    // 枚举所有可能的小立方体
    for (int i = 0; i < (int)x_arr.size() - 1; i++) {
        for (int j = 0; j < (int)y_arr.size() - 1; j++) {
            for (int k = 0; k < (int)z_arr.size() - 1; k++) {
                // 当前小立方体的范围
                int x1 = x_arr[i], x2 = x_arr[i + 1];
                int y1 = y_arr[j], y2 = y_arr[j + 1];
                int z1 = z_arr[k], z2 = z_arr[k + 1];
                
                // 检查这个小立方体是否被至少一个卫星覆盖
                bool covered = false;
                for (int s = 0; s < n; s++) {
                    int sx = satellites[s].x, sy = satellites[s].y, sz = satellites[s].z, sr = satellites[s].r;
                    
                    // 检查小立方体是否完全在卫星s的覆盖范围内
                    if (x1 >= sx - sr && x2 <= sx + sr &&
                        y1 >= sy - sr && y2 <= sy + sr &&
                        z1 >= sz - sr && z2 <= sz + sr) {
                        covered = true;
                        break;
                    }
                }
                
                if (covered) {
                    // 计算小立方体的体积并累加
                    long long volume = (long long)(x2 - x1) * (y2 - y1) * (z2 - z1);
                    total_volume += volume;
                }
            }
        }
    }
    
    cout << total_volume << endl;
    
    return 0;
} 