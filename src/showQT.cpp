#include "showQT.h"

void QtReadResult(vector<Bump> &bumps, const string &fileName, vector<double> &routing_area_coordinate) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cerr << "Can't open the file！" << endl;
        return;
    }

    string line;
    int lineCount = 0;
    while (getline(file, line)) {
        istringstream iss(line);
        if (lineCount < 2) {
            // 處理前兩行的數值資料
            double value1, value2;
            if (!(iss >> value1 >> value2)) {
                cerr << "Routing area parse error：" << line << endl;
                continue;
            }
            routing_area_coordinate.push_back(value1);
            routing_area_coordinate.push_back(value2);
        }else{
            Bump data;
            if (!(iss >> data.BumpCellName >> data.x >> data.y >> data.NetName >> data.isPG)) {
                cerr << "Parse errors：" << line << endl;
                continue;
            }

            cout << data.BumpCellName << " " << data.x << " " << data.y << " " << data.NetName << " " << data.isPG << endl;
            bumps.push_back(data);
        }

        lineCount++;
    }
    file.close();
}

void QtReadDesignRule(DesignRule &designRule, const string &fileName) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cerr << "Can't open the file！" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string key;
        if (!(iss >> key)) {
            cerr << "Parse errors：" << line << endl;
            continue;
        }
        if (key == "via_opening_diameter") {
            iss >> designRule.via_opening_diameter;
        } else if (key == "via_pad_diameter") {
            iss >> designRule.via_pad_diameter;
        } else if (key == "minimum_via_pad_spacing") {
            iss >> designRule.minimum_via_pad_spacing;
        } else if (key == "minimum_via_spacing") {
            iss >> designRule.minimum_via_spacing;
        } else if (key == "minimum_line_width") {
            iss >> designRule.minimum_line_width;
        } else if (key == "minimum_line_spacing") {
            iss >> designRule.minimum_line_spacing;
        } else if (key == "minimum_line_via_spacing") {
            iss >> designRule.minimum_line_via_spacing;
        } else {
            cerr << "Unknown key：" << key << endl;
        }
    }
    cout << "\nDesign Rules：" << endl;
    cout << "via_opening_diameter: " << designRule.via_opening_diameter << endl;
    cout << "via_pad_diameter: " << designRule.via_pad_diameter << endl;
    cout << "minimum_via_pad_spacing: " << designRule.minimum_via_pad_spacing << endl;
    cout << "minimum_via_spacing: " << designRule.minimum_via_spacing << endl;
    cout << "minimum_line_width: " << designRule.minimum_line_width << endl;
    cout << "minimum_line_spacing: " << designRule.minimum_line_spacing << endl;
    cout << "minimum_line_via_spacing: " << designRule.minimum_line_via_spacing << endl;

    file.close();
}

void QtDraw(QGraphicsScene *scene, const string &resultFile, const string &designRuleFile) {
    vector<Bump> bumps;
    vector<double> routing_area_coordinate;  // min_x, min_y, max_x, max_y
    DesignRule designRule;
    QtReadResult(bumps, resultFile, routing_area_coordinate);
    QtReadDesignRule(designRule, designRuleFile);

    // 設定緩衝區策略
    const double via_buffer_distance = designRule.via_opening_diameter / 2;
    const double track_buffer_distance = designRule.minimum_line_width / 2;

    const int points_per_circle = 36;
    boost::geometry::strategy::buffer::distance_symmetric<double> via_distance_strategy(via_buffer_distance);
    boost::geometry::strategy::buffer::distance_symmetric<double> track_distance_strategy(track_buffer_distance);
    boost::geometry::strategy::buffer::join_round join_strategy(points_per_circle);
    boost::geometry::strategy::buffer::end_round end_strategy(points_per_circle);
    boost::geometry::strategy::buffer::point_circle circle_strategy(points_per_circle);
    boost::geometry::strategy::buffer::side_straight side_strategy;

    // signal multipoint
    string signal_wkt = "MULTIPOINT(";
    for (const auto &bump : bumps)
        if (!bump.isPG)
            signal_wkt += "(" + to_string(bump.x) + " " + to_string(bump.y) + "),";
    signal_wkt[signal_wkt.size() - 1] = ')';

    // pg multipoint
    string pg_wkt = "MULTIPOINT(";
    for (const auto &bump : bumps)
        pg_wkt += "(" + to_string(bump.x) + " " + to_string(bump.y) + "),";
    pg_wkt[pg_wkt.size() - 1] = ')';

    
    multi_point signal_mp;
    boost::geometry::read_wkt(signal_wkt, signal_mp);
    
    // signal multipoint buffer
    multi_polygon result_signal_mp;
    boost::geometry::buffer(signal_mp, result_signal_mp, via_distance_strategy, side_strategy, join_strategy, end_strategy, circle_strategy);

   
    multi_point pg_mp;
    boost::geometry::read_wkt(pg_wkt, pg_mp);

     // pg multipoint buffer
    multi_polygon result_pg_mp;
    boost::geometry::buffer(pg_mp, result_pg_mp, via_distance_strategy, side_strategy, join_strategy, end_strategy, circle_strategy);

    // 繪製 pg 緩衝區 (黃色)
    QBrush pgBrush(Qt::yellow);
    for (const auto &poly : result_pg_mp) {
        QPolygonF qpoly;
        for (const auto &pt : poly.outer())
            qpoly << QPointF(pt.x(), pt.y());
        scene->addPolygon(qpoly, QPen(Qt::yellow), pgBrush);
    }

    // 繪製 signal 緩衝區 (黃色)
    QBrush signalBrush(Qt::yellow);
    for (const auto &poly : result_signal_mp) {
        QPolygonF qpoly;
        for (const auto &pt : poly.outer())
            qpoly << QPointF(pt.x(), pt.y());
        scene->addPolygon(qpoly, QPen(Qt::yellow), signalBrush);
    }

    // 繪製 pg 原始點 (藍色)
    for (const auto &pt : pg_mp) {
        scene->addEllipse(pt.x() - 5, pt.y() - 5, 10, 10, QPen(Qt::blue), QBrush(Qt::blue));
    }

    // 繪製 signal 原始點 (紅色)
    for (const auto &pt : signal_mp) {
        scene->addEllipse(pt.x() - 5, pt.y() - 5, 10, 10, QPen(Qt::red), QBrush(Qt::red));
    }


    //--------------------------------------------------------------------------------

    // 定義 LineString
    linestring ls;
    string ls_wkt = "LINESTRING(" + to_string(routing_area_coordinate[0]) + " " + to_string(routing_area_coordinate[1]) + "," + to_string(routing_area_coordinate[0]) + " " + to_string(routing_area_coordinate[3]) + "," + to_string(routing_area_coordinate[2]) + " " + to_string(routing_area_coordinate[3]) + "," + to_string(routing_area_coordinate[2]) + " " + to_string(routing_area_coordinate[1]) + "," + to_string(routing_area_coordinate[0]) + " " + to_string(routing_area_coordinate[1]) + ")";
    boost::geometry::read_wkt(ls_wkt , ls); //routing region

    // 繪製 LineString (綠色) => routing region
    QPolygonF qpoly;
    for (const auto &pt : ls) {
        qpoly << QPointF(pt.x(), pt.y());
    }
    scene->addPolygon(qpoly, QPen(Qt::green), QBrush(Qt::NoBrush));
}