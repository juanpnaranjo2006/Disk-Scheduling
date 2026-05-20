#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <fstream>
#include <chrono>
#include <cstdlib>

using namespace std;

const int N_REQUESTS = 1000;
const int N_CYLINDERS = 5000;

vector<int> generateRandRequests(int infLimit, int supLimit, int nRequests) {
    vector<int> values;

    for (int i = infLimit; i < supLimit; i++) {
        values.push_back(i);
    }

    unsigned seed =
        chrono::high_resolution_clock::now()
        .time_since_epoch()
        .count();

    mt19937 g(seed);

    shuffle(values.begin(), values.end(), g);

    values.resize(nRequests);

    return values;
}

bool elementsOrder(int a, int b, bool reverse) {
    bool ans = true;

    if (!reverse) {
        ans = a > b;
    } 
    else {
        ans = a < b;
    }

    return ans;
}

void deleteRepetitions(vector<int>& requests) {
    int i = 1;

    while (i < requests.size()) {
        if (requests[i] == requests[i - 1]) {
            requests.erase(requests.begin() + i);
        } 
        else {
            i++;
        }
    }
}

vector<int> scanOrder(int diskHeadInitialPos, vector<int> requests, int nCylinders, bool upwardsInitialDirection = true) {

    vector<int> orderedRequests;

    for (int req : requests) {
        if (req == diskHeadInitialPos || elementsOrder(req, diskHeadInitialPos, !upwardsInitialDirection)) {
            orderedRequests.push_back(req);
        }
    }

    sort(orderedRequests.begin(), orderedRequests.end(), greater<int>());

    if (upwardsInitialDirection) {
        reverse(orderedRequests.begin(), orderedRequests.end());
    }

    if (orderedRequests.size() < requests.size()) {

        vector<int> altRequests;

        if (upwardsInitialDirection && (orderedRequests.empty() || orderedRequests.back() != (nCylinders - 1))) {
            orderedRequests.push_back(nCylinders - 1);
        } 
        else if (!upwardsInitialDirection && (orderedRequests.empty() ||orderedRequests.back() != 0)) {
            orderedRequests.push_back(0);
        }

        for (int req : requests) {
            if (elementsOrder(req, diskHeadInitialPos, upwardsInitialDirection)) {
                altRequests.push_back(req);
            }
        }

        sort(altRequests.begin(), altRequests.end(), greater<int>());

        if (!upwardsInitialDirection) {
            reverse(altRequests.begin(), altRequests.end());
        }

        orderedRequests.insert(orderedRequests.end(), altRequests.begin(), altRequests.end());
    }

    orderedRequests.insert(orderedRequests.begin(), diskHeadInitialPos);

    deleteRepetitions(orderedRequests);

    return orderedRequests;
}

vector<int> c_scanOrder(int diskHeadInitialPos, vector<int> requests, int nCylinders, bool upwardsInitialDirection = true) {

    vector<int> orderedRequests;

    for (int req : requests) {
        if (req == diskHeadInitialPos || elementsOrder(req, diskHeadInitialPos, !upwardsInitialDirection)) {
            orderedRequests.push_back(req);
        }
    }

    sort(orderedRequests.begin(), orderedRequests.end(), greater<int>());

    if (upwardsInitialDirection) {
        reverse(orderedRequests.begin(), orderedRequests.end());
    }

    if (orderedRequests.size() < requests.size()) {

        vector<int> altRequests;

        if (upwardsInitialDirection && (orderedRequests.empty() || orderedRequests.back() != (nCylinders - 1))) {
            orderedRequests.push_back(nCylinders - 1);
            orderedRequests.push_back(0);
        } 
        else if (upwardsInitialDirection) {
            orderedRequests.push_back(0);
        } 
        else if (!upwardsInitialDirection && (orderedRequests.empty() || orderedRequests.back() != 0)) {
            orderedRequests.push_back(0);
            orderedRequests.push_back(nCylinders - 1);

        } 
        else if (!upwardsInitialDirection) {
            orderedRequests.push_back(nCylinders - 1);
        }

        for (int req : requests) {
            if (elementsOrder(req, diskHeadInitialPos, upwardsInitialDirection)) {
                altRequests.push_back(req);
            }
        }

        sort(altRequests.begin(), altRequests.end(), greater<int>());

        if (upwardsInitialDirection) {
            reverse(altRequests.begin(), altRequests.end());
        }

        orderedRequests.insert(orderedRequests.end(), altRequests.begin(), altRequests.end()
        );
    }

    orderedRequests.insert(orderedRequests.begin(), diskHeadInitialPos);

    deleteRepetitions(orderedRequests);

    return orderedRequests;
}

int totalHeadMovement(vector<int> requests) {
    int totalMovement = 0;

    if (!requests.empty()) {

        int prevPos = requests[0];

        for (int req : requests) {
            totalMovement += abs(req - prevPos);
            prevPos = req;
        }
    }

    return totalMovement;
}

void printVector(vector<int> v) {
    cout << "[";

    for (int i = 0; i < v.size(); i++) {
        cout << v[i];

        if (i != v.size() - 1) {
            cout << ", ";
        }
    }

    cout << "]";
}

/* ========================================================= */
/* ================= SVG IMPLEMENTATION ==================== */
/* ========================================================= */

void svgArrow(ofstream& svg,
              double x1,
              double y1,
              double x2,
              double y2,
              string color = "black") {

    svg << "<line "
        << "x1=\"" << x1 << "\" "
        << "y1=\"" << y1 << "\" "
        << "x2=\"" << x2 << "\" "
        << "y2=\"" << y2 << "\" "
        << "stroke=\"" << color << "\" "
        << "stroke-width=\"3\" "
        << "marker-end=\"url(#arrow)\" />\n";
}

void svgCircle(ofstream& svg,
               double x,
               double y,
               double r,
               string color = "white") {

    svg << "<circle "
        << "cx=\"" << x << "\" "
        << "cy=\"" << y << "\" "
        << "r=\"" << r << "\" "
        << "fill=\"" << color << "\" "
        << "stroke=\"black\" "
        << "stroke-width=\"2\" />\n";
}

void svgText(ofstream& svg,
             double x,
             double y,
             string text,
             int size = 18) {

    svg << "<text "
        << "x=\"" << x << "\" "
        << "y=\"" << y << "\" "
        << "font-size=\"" << size << "\" "
        << "text-anchor=\"middle\" "
        << "dominant-baseline=\"middle\">"
        << text
        << "</text>\n";
}

void generateAlgorithmSVG(vector<int> order, string filename, string title, int totalMovement) {

    const int WIDTH = 2400;
    const int HEIGHT = 1400;

    const int LEFT_MARGIN = 120;
    const int RIGHT_MARGIN = 40;

    const int TOP_MARGIN = 80;
    const int BOTTOM_MARGIN = 100;

    const int graphWidth =
        WIDTH - LEFT_MARGIN - RIGHT_MARGIN;

    const int graphHeight =
        HEIGHT - TOP_MARGIN - BOTTOM_MARGIN;

    ofstream svg("results/" + filename);

    if (!svg.is_open()) {
        cout << "Error creating file: "
             << filename << "\n";
        return;
    }

    svg << "<svg "
        << "width=\"" << WIDTH << "\" "
        << "height=\"" << HEIGHT << "\" "
        << "xmlns=\"http://www.w3.org/2000/svg\">\n";

    /* ================= BACKGROUND ================= */

    svg << R"(

<rect width="100%" height="100%" fill="white"/>

<style>

text {
    font-family: Arial;
}

</style>

)";

    /* ================= TITLE ================= */

    svg << "<text "
        << "x=\"" << WIDTH / 2 << "\" "
        << "y=\"40\" "
        << "font-size=\"32\" "
        << "font-weight=\"bold\" "
        << "text-anchor=\"middle\">"
        << title
        << "</text>\n";

    /* ================= MOVEMENT BOX ================= */

    const int BOX_WIDTH = 340;
    const int BOX_HEIGHT = 70;

    const int BOX_X =
        WIDTH - BOX_WIDTH - 50;

    const int BOX_Y = 25;

    svg << "<rect "
        << "x=\"" << BOX_X << "\" "
        << "y=\"" << BOX_Y << "\" "
        << "width=\"" << BOX_WIDTH << "\" "
        << "height=\"" << BOX_HEIGHT << "\" "
        << "rx=\"12\" "
        << "fill=\"#f0f0f0\" "
        << "stroke=\"#444\" "
        << "stroke-width=\"2\" />\n";

    svg << "<text "
        << "x=\"" << BOX_X + BOX_WIDTH / 2 << "\" "
        << "y=\"" << BOX_Y + BOX_HEIGHT / 2 + 5 << "\" "
        << "font-size=\"28\" "
        << "font-weight=\"bold\" "
        << "text-anchor=\"middle\">"
        << "Total Movement: "
        << totalMovement
        << "</text>\n";

    /* ================= AXES ================= */

    /* Y axis */
    svg << "<line "
        << "x1=\"" << LEFT_MARGIN << "\" "
        << "y1=\"" << TOP_MARGIN << "\" "
        << "x2=\"" << LEFT_MARGIN << "\" "
        << "y2=\"" << HEIGHT - BOTTOM_MARGIN << "\" "
        << "stroke=\"black\" "
        << "stroke-width=\"2\" />\n";

    /* X axis */
    svg << "<line "
        << "x1=\"" << LEFT_MARGIN << "\" "
        << "y1=\"" << HEIGHT - BOTTOM_MARGIN << "\" "
        << "x2=\"" << WIDTH - RIGHT_MARGIN << "\" "
        << "y2=\"" << HEIGHT - BOTTOM_MARGIN << "\" "
        << "stroke=\"black\" "
        << "stroke-width=\"2\" />\n";

    /* ================= GRID ================= */

    const int Y_DIVISIONS = 5;
    const int X_DIVISIONS = 5;

    /* Y GRID */
    for (int i = 0; i <= Y_DIVISIONS; i++) {

        double ratio =
            (double)i / Y_DIVISIONS;

        double y =
            HEIGHT - BOTTOM_MARGIN -
            ratio * graphHeight;

        int cylinder =
            ratio * (N_CYLINDERS - 1);

        svg << "<line "
            << "x1=\"" << LEFT_MARGIN << "\" "
            << "y1=\"" << y << "\" "
            << "x2=\"" << WIDTH - RIGHT_MARGIN << "\" "
            << "y2=\"" << y << "\" "
            << "stroke=\"#dddddd\" "
            << "stroke-width=\"1\" />\n";

        svg << "<text "
            << "x=\"" << LEFT_MARGIN - 20 << "\" "
            << "y=\"" << y + 5 << "\" "
            << "font-size=\"18\" "
            << "text-anchor=\"end\">"
            << cylinder
            << "</text>\n";
    }

    /* X GRID */
    for (int i = 0; i <= X_DIVISIONS; i++) {

        double ratio =
            (double)i / X_DIVISIONS;

        double x =
            LEFT_MARGIN +
            ratio * graphWidth;

        int requestIndex =
            ratio * (order.size() - 1);

        svg << "<line "
            << "x1=\"" << x << "\" "
            << "y1=\"" << TOP_MARGIN << "\" "
            << "x2=\"" << x << "\" "
            << "y2=\"" << HEIGHT - BOTTOM_MARGIN << "\" "
            << "stroke=\"#dddddd\" "
            << "stroke-width=\"1\" />\n";

        svg << "<text "
            << "x=\"" << x << "\" "
            << "y=\"" << HEIGHT - BOTTOM_MARGIN + 30 << "\" "
            << "font-size=\"18\" "
            << "text-anchor=\"middle\">"
            << requestIndex
            << "</text>\n";
    }

    /* ================= PATH ================= */

    for (int i = 0; i + 1 < order.size(); i++) {

        double x1 =
            LEFT_MARGIN +
            ((double)i / (order.size() - 1))
            * graphWidth;

        double x2 =
            LEFT_MARGIN +
            ((double)(i + 1) / (order.size() - 1))
            * graphWidth;

        double y1 =
            HEIGHT - BOTTOM_MARGIN -
            ((double)order[i] / (N_CYLINDERS - 1))
            * graphHeight;

        double y2 =
            HEIGHT - BOTTOM_MARGIN -
            ((double)order[i + 1] / (N_CYLINDERS - 1))
            * graphHeight;

        svg << "<line "
            << "x1=\"" << x1 << "\" "
            << "y1=\"" << y1 << "\" "
            << "x2=\"" << x2 << "\" "
            << "y2=\"" << y2 << "\" "
            << "stroke=\"blue\" "
            << "stroke-width=\"0.5\" "
            << "stroke-opacity=\"0.5\" />\n";
    }

    /* ================= POINTS ================= */

    for (int i = 0; i < order.size(); i++) {

        double x =
            LEFT_MARGIN +
            ((double)i / (order.size() - 1))
            * graphWidth;

        double y =
            HEIGHT - BOTTOM_MARGIN -
            ((double)order[i] / (N_CYLINDERS - 1))
            * graphHeight;

        svg << "<circle "
            << "cx=\"" << x << "\" "
            << "cy=\"" << y << "\" "
            << "r=\"0.8\" "
            << "fill=\"red\" "
            << "fill-opacity=\"0.8\" />\n";
    }

    /* ================= START/END ================= */

    if (!order.empty()) {

        /* START */
        {
            double x = LEFT_MARGIN;

            double y =
                HEIGHT - BOTTOM_MARGIN -
                ((double)order[0] / (N_CYLINDERS - 1))
                * graphHeight;

            svg << "<circle "
                << "cx=\"" << x << "\" "
                << "cy=\"" << y << "\" "
                << "r=\"5\" "
                << "fill=\"green\" />\n";
        }

        /* END */
        {
            double x =
                LEFT_MARGIN + graphWidth;

            double y =
                HEIGHT - BOTTOM_MARGIN -
                ((double)order.back() / (N_CYLINDERS - 1))
                * graphHeight;

            svg << "<circle "
                << "cx=\"" << x << "\" "
                << "cy=\"" << y << "\" "
                << "r=\"5\" "
                << "fill=\"red\" />\n";
        }
    }

    /* ================= AXIS TITLES ================= */

    svg << "<text "
        << "x=\"" << WIDTH / 2 << "\" "
        << "y=\"" << HEIGHT - 25 << "\" "
        << "font-size=\"24\" "
        << "text-anchor=\"middle\">"
        << "Request Number"
        << "</text>\n";

    svg << "<text "
        << "x=\"40\" "
        << "y=\"" << HEIGHT / 2 << "\" "
        << "font-size=\"24\" "
        << "text-anchor=\"middle\" "
        << "transform=\"rotate(-90 40,"
        << HEIGHT / 2
        << ")\">"
        << "Cylinder"
        << "</text>\n";

    svg << "</svg>";

    svg.close();
}

void generateHTMLReport() {

    ofstream html("results/index.html");

    html << R"(

<!DOCTYPE html>

<html>

<head>

<meta charset="UTF-8">

<title>Disk Scheduling Algorithms</title>

<style>

body {
    font-family: Arial;
    background-color: #f5f5f5;
    margin: 40px;
}

h1 {
    text-align: center;
}

.graph-container {
    background: white;
    padding: 20px;
    margin-bottom: 40px;
    border-radius: 10px;
    box-shadow: 0px 0px 10px rgba(0,0,0,0.1);
}

img {
    width: 100%;
    max-width: 1200px;
    border: 1px solid #ccc;
}

</style>

</head>

<body>

<h1>Disk Scheduling Algorithms</h1>

<div class="graph-container">
    <h2>FCFS</h2>
    <img src="fcfs.svg">
</div>

<div class="graph-container">
    <h2>SCAN UP</h2>
    <img src="scan_up.svg">
</div>

<div class="graph-container">
    <h2>SCAN DOWN</h2>
    <img src="scan_down.svg">
</div>

<div class="graph-container">
    <h2>C-SCAN UP</h2>
    <img src="cscan_up.svg">
</div>

<div class="graph-container">
    <h2>C-SCAN DOWN</h2>
    <img src="cscan_down.svg">
</div>

</body>

</html>

)";

    html.close();
}

/* ========================================================= */

int main(int argc, char* argv[]) {

    #ifdef _WIN32
        system("mkdir results >nul 2>nul");
    #else
        system("mkdir -p results");
    #endif

    if (argc <= 1) {

        cout << "Missing arguments! " << "Provide the initial position " << "for the disk head as parameter\n";

    } 
    else {

        int diskHeadInitialPos = stoi(argv[1]);

        if (diskHeadInitialPos < 0 || diskHeadInitialPos >= N_CYLINDERS) {
            cout << "Invalid Initial Position for Disk Head!\n";
            cout << "Initial Cylinder: 0\n";
            cout << "Maximum Cylinder: " << N_CYLINDERS - 1 << "\n";
            cout << "Disk Head Initial Position: " << diskHeadInitialPos << "\n";
            return 0;
        }

        vector<int> requests = generateRandRequests(0, N_CYLINDERS, N_REQUESTS);

        cout << "Requests:\n";
        //printVector(requests);
        cout << "\n\n";

        vector<int> scanRequestsUP = scanOrder(diskHeadInitialPos, requests, N_CYLINDERS);

        vector<int> scanRequestsDOWN = scanOrder(diskHeadInitialPos, requests, N_CYLINDERS, false);

        vector<int> c_scanRequestsUP = c_scanOrder(diskHeadInitialPos, requests, N_CYLINDERS);

        vector<int> c_scanRequestsDOWN = c_scanOrder(diskHeadInitialPos, requests, N_CYLINDERS, false);

        requests.insert(requests.begin(), diskHeadInitialPos);
        deleteRepetitions(requests);

        int totalMovFCFS = totalHeadMovement(requests);

        int totalMovScanUP = totalHeadMovement(scanRequestsUP);

        int totalMovScanDOWN = totalHeadMovement(scanRequestsDOWN);

        int totalMovC_ScanUP = totalHeadMovement(c_scanRequestsUP);

        int totalMovC_ScanDOWN = totalHeadMovement(c_scanRequestsDOWN);

        cout << "Algorithms:\n\n";

        cout << "FCFS\n";
        //printVector(requests);
        cout << " : Total Mov: " << totalMovFCFS << "\n\n";

        cout << "SCAN UP\n";
        //printVector(scanRequestsUP);
        cout << " : Total Mov: " << totalMovScanUP << "\n\n";

        cout << "SCAN DOWN\n";
        //printVector(scanRequestsDOWN);
        cout << " : Total Mov: " << totalMovScanDOWN << "\n\n";

        cout << "C-SCAN UP\n";
        //printVector(c_scanRequestsUP);
        cout << " : Total Mov: " << totalMovC_ScanUP << "\n\n";

        cout << "C-SCAN DOWN\n";
        //printVector(c_scanRequestsDOWN);
        cout << " : Total Mov: " << totalMovC_ScanDOWN << "\n";

        generateAlgorithmSVG(requests,
                             "fcfs.svg",
                             "FCFS", totalMovFCFS);

        generateAlgorithmSVG(scanRequestsUP,
                             "scan_up.svg",
                             "SCAN UP", totalMovScanUP);

        generateAlgorithmSVG(scanRequestsDOWN,
                             "scan_down.svg",
                             "SCAN DOWN", totalMovScanDOWN);

        generateAlgorithmSVG(c_scanRequestsUP,
                             "cscan_up.svg",
                             "C-SCAN UP", totalMovC_ScanUP);

        generateAlgorithmSVG(c_scanRequestsDOWN,
                             "cscan_down.svg",
                             "C-SCAN DOWN", totalMovC_ScanDOWN);

        cout << "SVG graphs generated!\n";

        generateHTMLReport();
    }

    return 0;
}