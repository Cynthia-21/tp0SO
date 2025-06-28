package ar.edu.utn.frba.dds.fuenteestatica.models.repositories.impl;

import ar.edu.utn.frba.dds.fuenteestatica.models.domain.Hecho;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.FuenteEstatica;
import ar.edu.utn.frba.dds.fuenteestatica.models.repositories.IHechoRepository;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import org.springframework.stereotype.Repository;

@Repository
public class HechoRepository implements IHechoRepository {
  private List<Hecho> listaDeHechos;

  public HechoRepository(){
    this.listaDeHechos = new ArrayList<>();
  }

  public void save(Hecho unHecho){
    this.listaDeHechos.add(unHecho);
  }
  public List<Hecho> findAll(){
    return listaDeHechos;
  }
  public List<Hecho> filtrarPorArchivo(String nombreArchivo){
    return listaDeHechos.stream().filter(hecho -> hecho.getArchivoOrigen().getNombre().equals(nombreArchivo)).collect(Collectors.toList());
  }
}
