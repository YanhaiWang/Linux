#include <iostream>
#include <vector>
// #include <algorithm>
using namespace std;

int main() {

    vector<int> weight = {1, 3, 4};
    vector<int> value = {15, 20, 30};
    int bagSize = 4;

    // vector<vector<int>> dp(weight.size(), vector<int>(bagSize + 1, 0));
    
    // for(int j = weight[0]; j <= bagSize; j++) // 初始化
    //     dp[0][j] = value[0];

    // for(int i = 1; i < weight.size(); i++) {  // 遍历物品
    //     for(int j = 0; j <= bagSize; j++) {  // 遍历容量
    //         if(j < weight[i])
    //             dp[i][j] = dp[i - 1][j];
    //         else   
    //             dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - weight[i]] + value[i]);
    //     }
    // }

    // for(int j = 0; j <= bagSize; j++) { // 遍历容量
    //     for(int i = 1; i < weight.size(); i++) { // 遍历物品
    //         if(j < weight[i])
    //             dp[i][j] = dp[i - 1][j];
    //         else   
    //             dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - weight[i]] + value[i]);
    //     }
    // }

    vector<int> dp(bagSize + 1, 0);

    
    for(int j = 0; j <= bagSize; j++) { // 遍历容量
        // for(int j = bagSize; j >= weight[i]; j--) { // 遍历容量
        //     dp[j] = max(dp[j], dp[j - weight[i]] + value[i]); // 01背包
        // }
        for(int i = 0; i < weight.size(); i++) { // 遍历物品
            if(j >= weight[i])
                dp[j] = max(dp[j], dp[j - weight[i]] + value[i]);  // 完全背包
        }

        for(int j = 0; j <= bagSize; j++)
            cout << dp[j] << " ";
        cout << endl;
    }

    // for(int i = 0; i < weight.size(); i++) {
    //     for(int j = 0; j <= bagSize; j++) {
    //         cout << dp[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    return 0;
}