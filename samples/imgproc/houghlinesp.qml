import 'lcvcore' 1.0
import 'lcvimgproc' 1.0

Column{
    
    // This sample shows the usage of Hough Line 
    // transform to detect lines within an image
    
    id : root
    
    property string imagePath : codeDocument.path + '/../_images/caltech_buildings_DSCN0246.JPG'
    
    Rectangle{
        
        width : sc.width
        height : sc.height
        
        ImRead{
            id : sc
            file : root.imagePath
            Component.onCompleted : {
                width  = width / 4
                height = height / 4
            }
        }
        
        Resize{
            id : scrs
            input : sc.output
            fx : 0.25
            fy : 0.25
        }
        
        HoughLinesP{
            input : cny.output
            width : sc.width
            height : sc.height
            rho : 0.5
            theta : Math.PI / 180
            threshold : 50
            lineThickness : 2
            lineColor : "#00ff00"
        }
    }
        
    Canny{
        id : cny
        input : scrs.output
        width : sc.width
        height : sc.height
        threshold1 : 70
        threshold2 : 210
        apertureSize : 3
    }
}
