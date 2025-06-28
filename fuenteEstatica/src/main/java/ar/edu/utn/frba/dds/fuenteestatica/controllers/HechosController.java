package ar.edu.utn.frba.dds.fuenteestatica.controllers;

import ar.edu.utn.frba.dds.fuenteestatica.models.domain.Hecho;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.dtos.output.HechoOutputDTO;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.Fuente;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.FuenteEstatica;
import ar.edu.utn.frba.dds.fuenteestatica.services.impl.FuenteEstaticaService;
import ar.edu.utn.frba.dds.fuenteestatica.services.impl.HechoService;
import java.util.List;
import java.util.Optional;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/servicioFuenteProxy")
public class HechosController {
  @Autowired
  FuenteEstaticaService fuenteService;
  @Autowired
  HechoService hechoService;

  @GetMapping("/{idFuente}/hechos")
  public List<Hecho> getAllHechos(
      @PathVariable String nombreArchivo,
      @RequestParam(required = false) Integer page,
      @RequestParam(required = false) Integer per_page){

      return hechoService.obtenerHechosAPartirDeArchivo(nombreArchivo);
  }
}
