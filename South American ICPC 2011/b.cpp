#include <iostream>

using namespace std;

int m[1010][1010];
int backup[1010][1010];
int sum[1010][1010];
int decision[1010][1010];
int sol[1010][1010];

int main() {
    int n;
    while(cin >> n) {
        if(!n)
            break;

        /* Reading */
        for( int i = 0 ; i < n ; ++i )
            for( int j = 0 ; j <= i ; ++j ) {
                cin >> m[i][j];
                backup[i][j] = m[i][j];
                decision[i][j] = 0;
                sol[i][j] = 0;
            }
                
        /* Calculating sums per column */
        sum[0][0] = m[0][0];
        for( int i = 1 ; i < n ; ++i )
            for( int j = 0 ; j <= i ; ++j )
                sum[i][j] = sum[i-1][j] + m[i][j];


        /*for( int i = 0 ; i < n ; ++i ) {
            for( int j = 0 ; j <= i ; ++j )
                cout << sum[i][j] << " ";
            cout << endl;
        }*/
                
        int result = 0;
        int resultJ = -1;
        /* Dealing with the first column */
        m[n-1][0] = sum[n-1][0];
        decision[n-1][0] = 1;
        for( int i = n-2 ; i >= 0 ; --i ) {
            if( m[i+1][0] > sum[i][0])
                m[i][0] = m[i+1][0];
            else {
                m[i][0] = sum[i][0];
                decision[i][0] = 1;
            }
        }
        if( m[0][0] > 0 ) {
            result = m[0][0];
            resultJ = 0;
        }
        
        /* Dealing with the rest of the columns */
        for( int j = 1 ; j < n ; ++j ) {
            m[n-1][j] = m[n-2][j-1] + sum[n-1][j];
            decision[n-1][j] = 1;
            for( int i = n-2 ; i >= j ; --i ) {
                if( m[i+1][j] > m[i-1][j-1] + sum[i][j])
                    m[i][j] = m[i+1][j];
                else {
                    m[i][j] = m[i-1][j-1] + sum[i][j];
                    decision[i][j] = 1;
                }
            }
            if( result < m[j][j] ) {
                result = m[j][j];
                resultJ = j;
            }
        }
        
        int i, j;
        i = j = resultJ;
        while( j >= 0 ) {
          sol[i][j] = 1;
          while( !decision[i][j] ) {
            ++i;
            sol[i][j] = 1;
          }
          --i;
          --j;
        }
        
        for( int i = n-1 ; i >= 1 ; --i ) {
            if( sol[i][0] )
                sol[i-1][0] = 1;
            if( sol[i][i] )
                sol[i-1][i-1] = 1;
            for( int j = 1 ; j < i ; ++j )
                if( sol[i][j] )
                    sol[i-1][j-1] = sol[i-1][j] = 1;
        }
        
        int check = 0;
        for( int i = 0 ; i < n ; ++i ) {
            for( int j = 0 ; j <= i ; ++j ) {
                check += sol[i][j] * backup[i][j];
                cout << sol[i][j] << " ";
            }
            cout << endl;
        }
        assert(check == result);
        
        cout << result << endl;
    }
    return 0;
}

