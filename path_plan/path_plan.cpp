
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#endif

using namespace std;
#ifdef USE_OPENCV
using namespace cv;
#endif

typedef struct {
    double x;
    double y;
} Point_t;

typedef vector<Point_t> Path_t;

double distance2D(Point_t pt1, Point_t pt2) {
    return sqrtf((pt1.x - pt2.x) * (pt1.x - pt2.x) + (pt1.y - pt2.y) * (pt1.y - pt2.y));
}

Point_t dstPt(Point_t base_node, Point_t next_node,
              double node_len, double base_offset) {
    Point_t dst_pt;
    dst_pt.x = base_node.x + (next_node.x - base_node.x) * base_offset / node_len;
    dst_pt.y = base_node.y + (next_node.y - base_node.y) * base_offset / node_len;
    return dst_pt;
}

Path_t pathPlan(Path_t input_nodes, int n) {
    Path_t output_path;
    vector<double> node_len;
    vector<double> milepost;  // len from node[0] to current node
    milepost.push_back(0.0);
    double sum_len = 0.0, unit_len, temp_len;
    for (int iloop = 0; iloop < input_nodes.size() -1; iloop++) {
        temp_len = distance2D(input_nodes[iloop], input_nodes[iloop + 1]);
        node_len.push_back(temp_len);
        sum_len += temp_len;
        milepost.push_back(sum_len);
    }
    unit_len = sum_len / (n + 1);
    double offset_len = 0.0;  // len from node[0] to last dst_pt
    for (int iloop = 0; iloop < input_nodes.size() -1; iloop++) {
        while (1) {
            if ((offset_len + unit_len) < milepost[iloop + 1]) {
                offset_len += unit_len;
                output_path.push_back(dstPt(input_nodes[iloop],
                                            input_nodes[iloop + 1],
                                            node_len[iloop],
                                            offset_len - milepost[iloop]));
            } else {
                break;
            }
        }
    }
    return output_path;
}

#ifdef USE_OPENCV
void showPathPlan(Path_t nodes, Path_t path) {
    Mat img(600, 1000, CV_8UC3, Scalar(0, 0, 0));
    for (int iloop = 0; iloop < nodes.size() -1; iloop++) {
        line(img, Point(nodes[iloop].x, nodes[iloop].y),
             Point(nodes[iloop + 1].x, nodes[iloop + 1].y), Scalar(255, 255, 0), 2, CV_AA);
    }
    for (int iloop = 0; iloop < nodes.size(); iloop++) {
        circle(img, Point(nodes[iloop].x, nodes[iloop].y), 5, Scalar(0, 255, 0), -1, CV_AA);
    }
    for (int iloop = 0; iloop < path.size(); iloop++) {
        circle(img, Point(path[iloop].x, path[iloop].y), 5, Scalar(0, 0, 255), -1, CV_AA);
    }
    imshow("path_plan", img);
    waitKey(-1);
}
#endif

int main(int argc, char *argv[]) {
    Path_t nodes, output_path;
    Point_t temp_pt;
    for (int iloop = 0; iloop < 5; iloop++) {
        temp_pt.x = rand() % 1000;
        temp_pt.y = rand() % 600;
        nodes.push_back(temp_pt);
    }

    output_path = pathPlan(nodes, 12);
#ifdef USE_OPENCV
    showPathPlan(nodes, output_path);
#endif
    return 0;
}