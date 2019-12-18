<?php
use App\Models\logger;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\DB;
/*
|--------------------------------------------------------------------------
| Application Routes
|--------------------------------------------------------------------------
|
| Here is where you can register all of the routes for an application.
| It is a breeze. Simply tell Lumen the URIs it should respond to
| and give it the Closure to call when that URI is requested.
|
*/

$router->get('/', function () use ($router) {
    return $router->app->version();
});
$router->get('/logger/add-record', function (Request $request)  {
    try{

        DB::beginTransaction();
      
        $device_id = $request->input('dev_id');
        $temperature = $request->input('temp');
        $humidity = $request->input('hum');
        $index_heat = $request->input('iheat');
        $log = new logger();
     
        $log->device_id = $device_id;
        $log->temperature = $temperature;
        $log->humidity = $humidity;
        $log->index_heat = $index_heat;
        $log->created_at = \Carbon\Carbon::now();
        $log->save();
        DB::commit();
        return 1;
    }catch(Exception $ex){
        DB::rollback();
        return $ex->getMessage();
        return 02;
    }
  
});